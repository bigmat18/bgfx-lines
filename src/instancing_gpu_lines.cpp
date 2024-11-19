#include <instancing_gpu_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>


namespace lines {
    InstancingGPULines::InstancingGPULines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "instancing_gpu_lines/vs_instancing_gpu_lines", "instancing_gpu_lines/fs_instancing_gpu_lines") 
    {
        m_TextureUniform = bgfx::createUniform("u_Texture", bgfx::UniformType::Sampler);
        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_IndirectBuffer = bgfx::createIndirectBuffer(1);

        m_ComputeProgram = bgfx::createProgram(vcl::loadShader("instancing_gpu_lines/cs_compute_texture"), true);
        m_ComputeIndirect = bgfx::createProgram(vcl::loadShader("instancing_gpu_lines/cs_compute_indirect"), true);

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

        allocateSegmentsBuffer(segments);
        allocateTextureBuffer(segments);
        generateTextureBuffer(segments);
        generateIndirectBuffer(segments);
    }

    InstancingGPULines::~InstancingGPULines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_SegmentsBuffer);
    }

    void InstancingGPULines::allocateTextureBuffer(const std::vector<Segment> &segments) {
        m_TextureBuffer = bgfx::createTexture2D(
            segments.size() * 3, 1, false, 1, bgfx::TextureFormat::RGBA32F, 
            BGFX_TEXTURE_COMPUTE_WRITE
        );
    }

    void InstancingGPULines::allocateSegmentsBuffer(const std::vector<Segment> &segments) {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_SegmentsBuffer = bgfx::createDynamicVertexBuffer(segments.size(), layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
        bgfx::update(m_SegmentsBuffer, 0, bgfx::makeRef(&segments[0], sizeof(Segment) * segments.size()));
    }

    void InstancingGPULines::generateTextureBuffer(const std::vector<Segment> &segments) {
        bgfx::setBuffer(0, m_SegmentsBuffer, bgfx::Access::Read);
        bgfx::setImage(1, m_TextureBuffer, 0, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeProgram, segments.size(), 1, 1);
    }

    void InstancingGPULines::generateIndirectBuffer(const std::vector<Segment> &segments) {
        float data[] = {static_cast<float>(segments.size()), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);
		bgfx::setBuffer(0, m_IndirectBuffer, bgfx::Access::Write);
		bgfx::dispatch(0, m_ComputeIndirect);
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
        
        bgfx::setTexture(0, m_TextureUniform, m_TextureBuffer);

        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(viewId, m_Program, m_IndirectBuffer, 0);
    }

    void InstancingGPULines::update(const std::vector<Segment> &segments) {
    }
}