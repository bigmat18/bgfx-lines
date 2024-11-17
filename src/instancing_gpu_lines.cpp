#include <instancing_gpu_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>


namespace lines {
    InstancingGPULines::InstancingGPULines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "instancing_gpu_lines/vs_test", "instancing_gpu_lines/fs_test") 
    {

        m_Vertices = {
            0.0f, 1.0f, 
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f, 
        };

        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()),
            layout
        );

        m_Indices = {
            0, 1, 2,
            1, 3, 2,
        };

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()),
            BGFX_BUFFER_INDEX32
        );

        m_InstancingUniform = bgfx::createUniform("u_Texture", bgfx::UniformType::Sampler);
        m_InstancingBuffer = bgfx::createTexture2D(
            segments.size() * 3, 1, false, 1, bgfx::TextureFormat::RGBA32F, 
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
        );


        bgfx::VertexLayout layout1;
        layout1
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_SegmentsBuffer = bgfx::createDynamicVertexBuffer(segments.size(), layout1, BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
        bgfx::update(m_SegmentsBuffer, 0, bgfx::makeRef(&segments[0], sizeof(Segment) * segments.size()));

        bgfx::allocInstanceDataBuffer(&m_DataBuffer, segments.size(), 16);

        // m_ComputeProgram = bgfx::createProgram(vcl::loadShader("instancing_gpu_lines/cs_compute_instancing_buffer"), true);
        // bgfx::setBuffer(0, m_SegmentsBuffer, bgfx::Access::Read);
        // bgfx::setTexture(1, m_InstancingUniform, m_InstancingBuffer);
        // bgfx::dispatch(0, m_ComputeProgram, segments.size(), 1, 1);
    }

    InstancingGPULines::~InstancingGPULines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_SegmentsBuffer);
    }

    void InstancingGPULines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.antialias, m_Data.thickness};
        bgfx::setUniform(m_UniformData, data);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        
        bgfx::setTexture(0, m_InstancingUniform, m_InstancingBuffer);

        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingGPULines::update(const std::vector<Segment> &segments) {
    }
}