/*
 * Copyright 2011-2024 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#define ENTRY_CONFIG_IMPLEMENT_MAIN 1
#include <bx/uint32_t.h>
#include <common.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include "logo.h"
#include <iostream>

namespace {

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
                init.type = bgfx::RendererType::OpenGL;
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

                imguiCreate();
                std::cout << bgfx::getRendererType() << std::endl;
            }

            virtual int shutdown() override {
                imguiDestroy();
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
                    imguiEndFrame();

                    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                    bgfx::touch(0);
                    bgfx::dbgTextClear();

                    const bgfx::Stats *stats = bgfx::getStats();
                    bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(stats->textWidth / 2), 20) - 20, 
                                       bx::max<uint16_t>(uint16_t(stats->textHeight / 2), 6) - 6, 
                                       40, 12, s_logo, 160);


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
        };
}

ENTRY_IMPLEMENT_MAIN(
    TestWindow, 
    "TestWindow", 
    "BGFX lines", 
    "BGFX lines");