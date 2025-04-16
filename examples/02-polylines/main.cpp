#define ENTRY_CONFIG_IMPLEMENT_MAIN 1
#include <bgfx_utils.h>
#include <common.h>
#include <camera.h>
#include <polylines.hpp>

#include "../common/generator.h"
namespace
{
    class ExamplePolylines : public entry::AppI
    {
    public:
        ExamplePolylines(const char *_name, const char *_description, const char *_url)
            : entry::AppI(_name, _description, _url) {}

        void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override
        {
            Args args(_argc, _argv);

            m_width = _width;
            m_height = _height;
            m_debug = BGFX_DEBUG_TEXT;
            m_reset = BGFX_RESET_VSYNC;

            bgfx::Init init;
            init.type = bgfx::RendererType::Direct3D11;
            init.vendorId = args.m_type;
            init.platformData.nwh = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
            init.platformData.ndt = entry::getNativeDisplayHandle();
            init.platformData.type = entry::getNativeWindowHandleType();
            init.resolution.width = m_width;
            init.resolution.height = m_height;
            init.resolution.reset = m_reset;
            bgfx::init(init);

            bgfx::setDebug(m_debug);
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFFFFFFFF, 1.0f, 0);

            cameraCreate();
            cameraSetPosition({0.0f, 0.0f, -5.0f});

            std::vector<lines::LinesVertex> points;
            generatePointsInCube(points, 3, 100);
            polyline = std::make_unique<lines::IndirectPolylines>(points);
            polyline->settings().setThickness(5);
            polyline->settings().setBorder(2);
            polyline->settings().setAntialias(0);
            polyline->settings().setLeftCap(lines::LineCap::TRIANGLE_CAP);
            polyline->settings().setRigthCap(lines::LineCap::ROUND_CAP);
            // polyline->settings().setJoint(lines::PolyLineJoint::BEVEL_JOINT);
            polyline->settings().setGeneralColor(lines::LinesVertex::COLOR(1, 0, 1, 1));
        }

        virtual int shutdown() override
        {
            bgfx::shutdown();
            return 0;
        }

        bool update() override
        {
            if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState))
            {
                cameraCoordinatesUpdate();
                float proj[16];
                // bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
                bx::mtxOrtho(proj, left, right, bottom, top, nearPlane, farPlane, 0.0f, false);
                
                float view[16];
                cameraGetViewMtx(view);

                bgfx::setViewTransform(0, view, proj);

                bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                bgfx::touch(0);
                polyline->draw(0);
                bgfx::frame();

                return true;
            }

            return false;
        }

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

        entry::MouseState m_mouseState;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_debug;
        uint32_t m_reset;

        uint64_t m_timeOffset;

        float left = -10.0f;
        float right = 10.0f;
        float bottom = -10.0f;
        float top = 10.0f;
        float nearPlane = -10.0f;
        float farPlane = 10.0f;

        std::unique_ptr<lines::IndirectPolylines> polyline;
    };
}

ENTRY_IMPLEMENT_MAIN(
    ExamplePolylines,
    "ExamplePolylines Window",
    "ExamplePolylines",
    "ExamplePolylines");