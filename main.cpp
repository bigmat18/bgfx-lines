#define ENTRY_CONFIG_IMPLEMENT_MAIN 1

#include <bx/uint32_t.h>
#include <iostream>
#include <common.h>
#include <camera.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include <math.h>
#include <vector>

#include <lines.hpp>
#include <triangulated_lines_handler.hpp>
#include <primitive_lines_handler.hpp>

namespace {

    #define DEBUG_PRINT() bx::debugPrintf("%s - %d", __FILE__, __LINE__)
    #define FROM_PX_TO_NORM(value, resolution) ((2.0f * value / resolution) - 1.0f)

    #define MTX_BASE    \
    {                   \
        1, 0, 0, 0,     \
        0, 1, 0, 0,     \
        0, 0, 1, 0,     \
        0, 0, 0, 1      \
    }

    inline void printMatrix(float* mtx) {
        for (uint32_t i = 0; i < 4; i++)
            std::cout << mtx[i] << " " << mtx[i + 4] << " " << mtx[i + 8] << " " << mtx[i + 12] << std::endl;
    }

    inline std::vector<float> linespace(int n, float start, float stop) {
        std::vector<float> T(n);
        float step = (stop - start) / (n - 1);

        for (int i = 0; i < n; ++i)
            T[i] = start + i * step;

        return T;
    }

    class Main : public entry::AppI {
        public:
            Main(const char *_name, const char *_description, const char *_url)
                : entry::AppI(_name, _description, _url)
            {}

            void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override {
                Args args(_argc, _argv);
                
                m_width = _width;
                m_height = _height;
                m_debug = BGFX_DEBUG_TEXT;
                m_reset = BGFX_RESET_VSYNC;

                bgfx::Init init;
                init.type = args.m_type;
                init.vendorId = args.m_pciId;
                init.platformData.nwh = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
                init.platformData.ndt = entry::getNativeDisplayHandle();
                init.platformData.type = entry::getNativeWindowHandleType();
                init.resolution.width = m_width;
                init.resolution.height = m_height;
                init.resolution.reset = m_reset;
                bgfx::init(init);

                bgfx::setDebug(m_debug);
                bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

                // ======= Camera setup ======
                cameraCreate();
                cameraSetPosition({0.0f, 0.0f, -5.0f});

                m_timeOffset = bx::getHPCounter();
                // ======= Camera setup ======


                imguiCreate();


                // ======= Lines setup ======
                createSpiralLines();
                createSphereLines();
                createTriangleLines();
                // ======= Lines setup ======

                m_angle = 0.0f;
                usePerspective = true;
            }

            virtual int shutdown() override {
                imguiDestroy();
                cameraDestroy();
                Lines::Shutdown();
                bgfx::shutdown();
                return 0;
            }

            bool update() override {
                if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState)) {

                    this->imguiRender();
                    this->cameraCoordinatesUpdate();
                    
                    float model[16] = MTX_BASE;
                    float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));
                    bx::mtxRotateZ(model, time);

                    m_angle += 0.1f;

                    float view[16];
                    cameraGetViewMtx(view);

                    float proj[16];
                    if(usePerspective)
                        bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
                    else 
                        bx::mtxOrtho(proj, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);

                    bgfx::setViewTransform(0, view, proj);
                    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                    bgfx::touch(0);


                    #if 0
                        std::cout << std::endl << "Model matrix" << std::endl;
                        printMatrix(model);

                        std::cout << std::endl << "View matrix" << std::endl;
                        printMatrix(view);

                        std::cout << std::endl << "Proj matrix" << std::endl;
                        printMatrix(proj);

                        float viewProj[16];
                        bx::mtxMul(viewProj, view, proj);
                        std::cout << std::endl << "View * Proj" << std::endl;
                        printMatrix(viewProj);

                        float modelViewProj[16];
                        bx::mtxMul(modelViewProj, model, viewProj);
                        std::cout << std::endl << "Model * View * Proj" << std::endl;
                        printMatrix(modelViewProj);

                        exit(0);
                    #endif 

                    uint64_t state = 0
                        | BGFX_STATE_WRITE_R
                        | BGFX_STATE_WRITE_G
                        | BGFX_STATE_WRITE_B
                        | BGFX_STATE_WRITE_A
                        | BGFX_STATE_WRITE_Z
                        | BGFX_STATE_DEPTH_TEST_ALWAYS
                        | BGFX_STATE_CULL_CW
                        | BGFX_STATE_MSAA
                        | UINT64_C(0)
                        | BGFX_STATE_BLEND_ALPHA;

                    // bgfx::setTransform(model);
                    Lines::Render(state);

                    bgfx::frame();

                    return true;
                }

                return false;
            }

        private:
            void cameraCoordinatesUpdate() {
                int64_t now = bx::getHPCounter();
                static int64_t last = now;
                const int64_t frameTime = now - last;
                last = now;

                const double freq = double(bx::getHPFrequency());
                const float time = (float)((now - m_timeOffset) / double(bx::getHPFrequency()));
                const float deltaTime = float(frameTime / freq);

                cameraUpdate(deltaTime * 0.1, m_mouseState);
            }

            void imguiRender() {
                imguiBeginFrame(m_mouseState.m_mx,
                                m_mouseState.m_my,
                                (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0) |
                                (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0) |
                                (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0),
                                m_mouseState.m_mz,
                                uint16_t(m_width),
                                uint16_t(m_height));

                Lines::DebugMenu();

                #if 1
                    float win_width = 350, win_heigth = 100;
                    ImGui::SetNextWindowPos(ImVec2({static_cast<float>(m_width) - win_width, 0}));
                    ImGui::SetNextWindowSize(ImVec2({win_width, win_heigth}));
                    ImGui::Begin("Custom debugging params", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
                    bx::Vec3 position = cameraGetPosition();
                    ImGui::Text("Camera Position: (%f, %f, %f)", position.x, position.y, position.z);

                    if (ImGui::RadioButton("Perspective", usePerspective)) {
                        usePerspective = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::RadioButton("Orthographic", !usePerspective)) {
                        usePerspective = false;
                    }

                    ImGui::End();
                #endif

                imguiEndFrame();
            }

            void createSpiralLines() {
                Lines::TriangulatedLinesHandler *triangulated_line = dynamic_cast<Lines::TriangulatedLinesHandler *>(
                    Lines::CreateHandler(Lines::LinesType::TRIANGULATED_LINES, "Spiral (triangulate version)")
                );
                triangulated_line->SetColor(1.0, 0.0, 1.0, 1.0);
                triangulated_line->SetResolution(m_width, m_height);
                triangulated_line->SetAntialis(1.5f);
                triangulated_line->SetThickness(5.0f);
                triangulated_line->SetActive(false);

                Lines::PrimitiveLinesHandler *primitive_line = dynamic_cast<Lines::PrimitiveLinesHandler *>(
                    Lines::CreateHandler(Lines::LinesType::PRIMITIVE_LINES, "Spiral (primitive version)")

                );

                primitive_line->SetColor(1.0, 0.0, 1.0, 1.0);
                primitive_line->SetActive(false);

                triangulated_line->BeginLine();
                primitive_line->BeginLine();

                int n = 1000;
                std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
                std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);

                for (int i = 0; i < n; i++)
                {
                    float X = cosf(T[i]) * R[i];
                    float Y = sinf(T[i]) * R[i];

                    triangulated_line->AddPoint(Lines::LinesPoint(X, Y, 0.0));
                    primitive_line->AddPoint(Lines::LinesPoint(X, Y, 0.0));
                }

                triangulated_line->EndLine();
                primitive_line->EndLine();
            }

            void createSphereLines() {
                Lines::TriangulatedLinesHandler *triangulated_line = dynamic_cast<Lines::TriangulatedLinesHandler *>(
                    Lines::CreateHandler(Lines::LinesType::TRIANGULATED_LINES, "Sphere (triangulate version)")
                )
                ;
                triangulated_line->SetColor(1.0, 0.0, 1.0, 1.0);
                triangulated_line->SetResolution(m_width, m_height);
                triangulated_line->SetAntialis(1.5f);
                triangulated_line->SetThickness(5.0f);
                triangulated_line->SetActive(false);

                Lines::PrimitiveLinesHandler *primitive_line = dynamic_cast<Lines::PrimitiveLinesHandler *>(
                    Lines::CreateHandler(Lines::LinesType::PRIMITIVE_LINES, "Sphere (primitive version)")
                );

                primitive_line->SetColor(1.0, 0.0, 1.0, 1.0);
                primitive_line->SetActive(false);

                triangulated_line->BeginLine();
                primitive_line->BeginLine();

                int n = 1000;

                std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
                std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);

                for (int i = 0; i < n; i++)
                {
                    float X = cosf(T[i]) * sinf(R[i]);
                    float Y = sinf(T[i]) * sinf(R[i]);
                    float Z = -100.0 + cosf(R[i]);

                    triangulated_line->AddPoint(Lines::LinesPoint(float(X), float(Y), float(Z)));
                    primitive_line->AddPoint(Lines::LinesPoint(float(X), float(Y), float(Z)));
                }

                triangulated_line->EndLine();
                primitive_line->EndLine();
            }

            void createTriangleLines() {
                Lines::TriangulatedLinesHandler *triangulated_line = dynamic_cast<Lines::TriangulatedLinesHandler *>(
                    Lines::CreateHandler(Lines::LinesType::TRIANGULATED_LINES, "Triangle (triangulate version)")
                );

                triangulated_line->SetColor(1.0, 0.0, 1.0, 1.0);
                triangulated_line->SetResolution(m_width, m_height);
                triangulated_line->SetAntialis(1.5f);
                triangulated_line->SetThickness(5.0f);
                triangulated_line->SetActive(false);

                Lines::PrimitiveLinesHandler *primitive_line = dynamic_cast<Lines::PrimitiveLinesHandler *>(
                    Lines::CreateHandler(Lines::LinesType::PRIMITIVE_LINES, "Triangle (primitive version)")
                );

                primitive_line->SetColor(1.0, 0.0, 1.0, 1.0);
                primitive_line->SetActive(false);

                triangulated_line->BeginLine();
                primitive_line->BeginLine();

                primitive_line->SetClosed(true);
                primitive_line->AddPoint(Lines::LinesPoint(-0.25f, -0.25f, 0.0f));
                primitive_line->AddPoint(Lines::LinesPoint(0.25f, -0.25f, 0.0f));
                primitive_line->AddPoint(Lines::LinesPoint(0.0f, 0.25f, 0.0f));
                primitive_line->AddPoint(Lines::LinesPoint(-0.25f, -0.25f, 0.0f));

                triangulated_line->AddPoint(Lines::LinesPoint(-0.25, -0.25, 0.0));
                triangulated_line->AddPoint(Lines::LinesPoint(0.25, -0.25, 0.0));
                triangulated_line->AddPoint(Lines::LinesPoint(0.0, 0.25, 0.0));
                triangulated_line->AddPoint(Lines::LinesPoint(-0.25f, -0.25f, 0.0f));

                triangulated_line->EndLine();
                primitive_line->EndLine();
            }

            entry::MouseState m_mouseState;

            uint32_t m_width;
            uint32_t m_height;
            uint32_t m_debug;
            uint32_t m_reset;
            uint64_t m_timeOffset;

            float m_angle;
            bool usePerspective;
    };
}

ENTRY_IMPLEMENT_MAIN(
    Main,
    "Main window",
    "Main",
    "Main"
);
