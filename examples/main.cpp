#define ENTRY_CONFIG_IMPLEMENT_MAIN 1
#include <bx/uint32_t.h>
#include <common.h>
#include <camera.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include <lines.h>
#include <math.h>

namespace {

    class BGFXLines : public entry::AppI {
        public:
            BGFXLines(const char *_name, const char *_description, const char *_url)
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

                horizontal_rotation = 0.0f;
                vertical_rotation = 0.0f;

                lastX = horizontal_rotation;
                lastY = vertical_rotation; 

                Lines::Init();
                cameraCreate();
                cameraSetPosition({0.0f, 0.0f, -5.0f});

                cameraSetHorizontalAngle(horizontal_rotation);
                cameraSetVerticalAngle(vertical_rotation);
                
                imguiCreate();

                m_timeOffset = bx::getHPCounter();

                Lines::SetResolution(m_width, m_height);
                Lines::SetThickness(5);
                Lines::SetAntialis(4);
                Lines::SetColor(1.0, 0.0, 1.0, 1.0);
            }

            virtual int shutdown() override {
                imguiDestroy();
                Lines::Shutdown();
                bgfx::shutdown();
                cameraDestroy();
                return 0;
            }

            bool update() override {
                if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState)) {
                    imguiBeginFrame(m_mouseState.m_mx, 
                                    m_mouseState.m_my, 
                                    (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0) | 
                                    (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0) | 
                                    (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0), 
                                    m_mouseState.m_mz, 
                                    uint16_t(m_width), 
                                    uint16_t(m_height));

                    showExampleDialog(this);
                    ImGui::SetNextWindowPos(
                        ImVec2(m_width - m_width / 5.0f - 10.0f, 10.0f), ImGuiCond_FirstUseEver);
                    ImGui::SetNextWindowSize(
                        ImVec2(m_width / 5.0f, m_height / 3.5f), ImGuiCond_FirstUseEver);

                    imguiEndFrame();

                    int64_t now = bx::getHPCounter();
                    static int64_t last = now;
                    const int64_t frameTime = now - last;
                    last = now;
                    const double freq = double(bx::getHPFrequency());
                    const float time = (float)((now - m_timeOffset) / double(bx::getHPFrequency()));
                    const float deltaTime = float(frameTime / freq);

                    float nowX = ((2.0f * m_mouseState.m_mx / m_width) - 1.0f);
                    float nowY = ((2.0f * m_mouseState.m_my / m_height) - 1.0f);

                    // bx::debugPrintf("(%d, %d)", m_mouseState.m_mx, m_mouseState.m_my);
                    // bx::debugPrintf("(%f, %f)", nowX, nowY);

                    float offsetX = nowX  - lastX;
                    float offsetY = lastY - nowY;

                    lastX = nowX;
                    lastY = nowY;

                    horizontal_rotation += offsetX * 2.0f;
                    vertical_rotation += offsetY * 2.0f;

                    cameraSetHorizontalAngle(horizontal_rotation);
                    cameraSetVerticalAngle(vertical_rotation);

                    cameraUpdate(deltaTime * 0.1, m_mouseState);

                    {
                        float view[16];
                        cameraGetViewMtx(view);

                        float proj[16];
                        bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
                        bgfx::setViewTransform(0, view, proj);

                        bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                    }

                    bgfx::touch(0);
                    float mtx[16] = MTX_BASE;
                    bgfx::setTransform(mtx);


                    uint64_t state = 0
                        | BGFX_STATE_WRITE_R
                        | BGFX_STATE_WRITE_G
                        | BGFX_STATE_WRITE_B
                        | BGFX_STATE_WRITE_A
                        | BGFX_STATE_WRITE_Z
                        | BGFX_STATE_DEPTH_TEST_ALWAYS
                        | BGFX_STATE_CULL_MASK
                        | BGFX_STATE_MSAA
                        | BGFX_STATE_PT_TRISTRIP
                        | BGFX_STATE_BLEND_ALPHA;


                    // Lines::RenderLines(0.0f, 0.0f, cosf(time) * (m_width / 2), sinf(time) * (m_height / 2));
                    for(float i = 0; i < 10; i++) {
                        Lines::SetThickness(i + 1);
                        Lines::RenderLines((i * 100.0) + 50.0, m_height * 0.25, (i * 100.0) + 300.0, m_height * 0.75, state);
                    }

                    bgfx::frame();

                    return true;
                }

                return false;
            }

            entry::MouseState m_mouseState;

            uint32_t m_width;
            uint32_t m_height;
            uint32_t m_debug;
            uint32_t m_reset;
            uint64_t m_timeOffset;

            float horizontal_rotation; // pitch
            float vertical_rotation; // yaw

            float lastX;
            float lastY;
    };
}

ENTRY_IMPLEMENT_MAIN(
    BGFXLines, 
    "BGFX Lines window", 
    "BGFX lines", 
    "BGFX lines"
);