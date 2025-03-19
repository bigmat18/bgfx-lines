#define ENTRY_CONFIG_IMPLEMENT_MAIN 1
#include <bgfx_utils.h>
#include <common.h>
#include <lines.hpp>

#include "../common/generator.h"
namespace
{
    class ExampleLines : public entry::AppI
    {
    public:
        ExampleLines(const char *_name, const char *_description, const char *_url)
            : entry::AppI(_name, _description, _url) {}

        void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override
        {
            Args args(_argc, _argv);

            m_width = _width;
            m_height = _height;
            m_debug = BGFX_DEBUG_TEXT;
            m_reset = BGFX_RESET_VSYNC;

            bgfx::Init init;
            init.type = bgfx::RendererType::Vulkan;
            init.vendorId = 0;
            init.platformData.nwh = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
            init.platformData.ndt = entry::getNativeDisplayHandle();
            init.resolution.width = m_width;
            init.resolution.height = m_height;
            init.resolution.reset = m_reset;
            bgfx::init(init);

            bgfx::setDebug(m_debug);
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFFFFFFFF, 1.0f, 0);

            std::vector<lines::LinesVertex> points;
            generatePointsInCube(points, 3, 1000);

            line = lines::Lines::create(points, lines::LinesTypes::TEXTURE_BASED);
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
                bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                bgfx::touch(0);

                line->draw(0);

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
        std::unique_ptr<lines::Lines> line;
    };
}

ENTRY_IMPLEMENT_MAIN(
    ExampleLines,
    "ExampleLines Window",
    "ExampleLines",
    "ExampleLines");