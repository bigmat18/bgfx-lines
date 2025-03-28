#define ENTRY_CONFIG_IMPLEMENT_MAIN 1
#include "common.h"
#include "bgfx_utils.h"
#include "imgui/imgui.h"
#include "camera.h"
#include <bgfx/bgfx.h>

namespace
{

    static const float s_quadVertices[] =
    {
        -0.5f,  -0.5f,
        -0.5f,  0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
    };

    static const uint16_t s_quadIndices[] = { 0, 3, 1, 0, 2, 3, };

    class ComputeTest : public entry::AppI
    {
    public:
        ComputeTest(const char *_name, const char *_description, const char *_url)
            : entry::AppI(_name, _description, _url) {}

        void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override
        {
            Args args(_argc, _argv);

            m_width = _width;
            m_height = _height;
            m_debug = BGFX_DEBUG_TEXT;
            m_reset = BGFX_RESET_VSYNC;

            bgfx::Init init;
            init.type     = args.m_type;
            init.vendorId = args.m_pciId;
            init.platformData.nwh = entry::getNativeWindowHandle(entry::kDefaultWindowHandle);
            init.platformData.ndt = entry::getNativeDisplayHandle();
            init.resolution.width = m_width;
            init.resolution.height = m_height;
            init.resolution.reset = m_reset;
            bgfx::init(init);

            bgfx::setDebug(m_debug);
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFFFFFFFF, 1.0f, 0);

            m_Program = loadProgram("test/vs_test", "test/fs_test");
            m_Compute = bgfx::createProgram(loadShader("test/cs_test"), true);

            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
                .end();

            m_VBO = bgfx::createVertexBuffer(
                bgfx::makeRef(nullptr, sizeof(s_quadVertices)), layout,
                BGFX_BUFFER_COMPUTE_READ_WRITE
            );

            m_Buffer = bgfx::createVertexBuffer(
                bgfx::makeRef(s_quadVertices, sizeof(s_quadVertices)), layout,
                BGFX_BUFFER_COMPUTE_READ_WRITE
            );

            m_IBO = bgfx::createIndexBuffer(
                bgfx::makeRef(s_quadIndices, sizeof(s_quadIndices)));

            bgfx::setBuffer(0, m_VBO, bgfx::Access::Write);
            bgfx::setBuffer(1, m_Buffer, bgfx::Access::Read);
            bgfx::dispatch(0, m_Compute, 1, 1, 1);
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

                bgfx::setVertexBuffer(0, m_VBO);
				bgfx::setIndexBuffer(m_IBO);

                bgfx::setState(
                    0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A  |
                    BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                    UINT64_C(0) | BGFX_STATE_BLEND_ALPHA
					);

                bgfx::submit(0, m_Program);


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

        bgfx::ProgramHandle m_Program = BGFX_INVALID_HANDLE;
        bgfx::ProgramHandle m_Compute = BGFX_INVALID_HANDLE;

        bgfx::VertexBufferHandle m_Buffer = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle m_VBO = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle m_IBO = BGFX_INVALID_HANDLE;
    };
}

ENTRY_IMPLEMENT_MAIN(
    ComputeTest,
    "Compute Window",
    "Compute",
    "Compute");