/*
 * Copyright 2011-2024 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#define ENTRY_CONFIG_IMPLEMENT_MAIN 1
#include <bx/uint32_t.h>
#include <common.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include "utils.h"


namespace {
    struct PosColorVertex {
        float m_x;
        float m_y;
        float m_z;
        uint32_t m_abgr;

        static void init() {
            ms_layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
        };

        static bgfx::VertexLayout ms_layout;
    };
    
    bgfx::VertexLayout PosColorVertex::ms_layout;

    static PosColorVertex s_cubeVertices[] = {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };
    

    static const uint16_t s_cubeTriList[] = {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };

    class TestWindow : public entry::AppI {
        public:
            TestWindow(const char *_name, const char *_description, const char *_url)
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

                PosColorVertex::init();

                std::vector<PosColorVertex> vertices;
                std::vector<uint16_t> indices;

                for(int y = 0; y < 100; ++y) {
                    for(int x = 0; x < 100; ++x) {
                        vertices.push_back({-1.0f + floorf(2.0f / 100.0f) * x, 1.0f * floorf(2.0f / 100.0f) * x, 0.0f, 0xff000000});
                        
                        if(y == 0 || x == 0) {
                            indices.push_back(x);
                            indices.push_back(x+1);
                            indices.push_back(y+1);
                        }

                        if(y != 100 && x != 0) {

                        }
                    }
                }

                m_vbh = bgfx::createVertexBuffer(
                    bgfx::makeRef(&vertices[0], vertices.size() * sizeof(vertices[0])),
                    PosColorVertex::ms_layout
                );

                m_ibh = bgfx::createIndexBuffer(
                    bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
                );

                m_program = loadProgram("vs_base", "fs_base");
                m_timeOffset = bx::getHPCounter();

                imguiCreate();
            }

            virtual int shutdown() override {
                imguiDestroy();

                bgfx::destroy(m_ibh);
                bgfx::destroy(m_vbh);
                bgfx::destroy(m_program);

                bgfx::shutdown();
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

                    float time = (float)((bx::getHPCounter() - m_timeOffset) / double(bx::getHPFrequency()));

                    const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
                    const bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

                    {
                        float view[16];
                        bx::mtxLookAt(view, eye, at);

                        float proj[16];
                        bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
                        bgfx::setViewTransform(0, view, proj);

                        bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                    }

                    bgfx::touch(0);

                    uint64_t state = 0
                        | BGFX_STATE_WRITE_R
                        | BGFX_STATE_WRITE_G
                        | BGFX_STATE_WRITE_B
                        | BGFX_STATE_WRITE_A
                        | BGFX_STATE_WRITE_Z
                        | BGFX_STATE_DEPTH_TEST_LESS
                        | BGFX_STATE_CULL_CW
                        | BGFX_STATE_MSAA
                        | UINT64_C(0);



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

            bgfx::ProgramHandle m_program;
            bgfx::VertexBufferHandle m_vbh;
            bgfx::IndexBufferHandle m_ibh;
    };
}

ENTRY_IMPLEMENT_MAIN(
    TestWindow, 
    "TestWindow", 
    "BGFX lines", 
    "BGFX lines"
);