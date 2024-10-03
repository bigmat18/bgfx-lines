#define ENTRY_CONFIG_IMPLEMENT_MAIN 1

#include <bx/uint32_t.h>
#include <iostream>
#include <common.h>
#include <camera.h>
#include <bgfx_utils.h>
#include <imgui/imgui.h>
#include <math.h>
#include <lines.h>

namespace {

    class BadLines : public entry::AppI {

        public:
            BadLines(const char *_name, const char *_description, const char *_url)
                : entry::AppI(_name, _description, _url)
            {
            }

            void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override
            {
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
                horizontal_rotation = 0.0f;
                vertical_rotation = 0.0f;

                lastX = horizontal_rotation;
                lastY = vertical_rotation;

                cameraCreate();
                cameraSetPosition({0.0f, 0.0f, -5.0f});
                cameraSetHorizontalAngle(horizontal_rotation);
                cameraSetVerticalAngle(vertical_rotation);

                m_timeOffset = bx::getHPCounter();
                // ======= Camera setup ======

                imguiCreate();

                // ======= Lines setup ======

                // ======= Lines setup ======
            }

            virtual int shutdown() override {
                return 0;
            }

            bool update() override {
                return true;
            }

        private:
            entry::MouseState m_mouseState;

            uint32_t m_width;
            uint32_t m_height;
            uint32_t m_debug;
            uint32_t m_reset;
            uint64_t m_timeOffset;

            float horizontal_rotation; // pitch
            float vertical_rotation;   // yaw

            float lastX;
            float lastY;
    };
}

ENTRY_IMPLEMENT_MAIN(
    BadLines,
    "Bad lines window",
    "Bad lines",
    "Bad lines"
);