#include <lines/indirect_based_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>


namespace lines {
    IndirectBasedLines::IndirectBasedLines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "lines/indirect_based_lines/vs_indirect_based_lines", "lines/indirect_based_lines/fs_indirect_based_lines"),
        m_SegmentsSize(segments.size())
    {
        m_IndirectBuffer = bgfx::createIndirectBuffer(1);
        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_ComputeIndirect = bgfx::createProgram(vcl::loadShader("lines/indirect_based_lines/cs_compute_indirect"), true);

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

        generateIndirectBuffer();
        allocateSegmentsBuffer();
        bgfx::update(m_SegmentsBuffer, 0, bgfx::makeRef(&segments[0], sizeof(Segment) * segments.size()));
    }

    IndirectBasedLines::~IndirectBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_SegmentsBuffer);
        bgfx::destroy(m_IndirectBuffer);
        bgfx::destroy(m_ComputeIndirect);
    }

    void IndirectBasedLines::allocateSegmentsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_SegmentsBuffer = bgfx::createDynamicVertexBuffer(m_SegmentsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void IndirectBasedLines::generateIndirectBuffer() {
        float data[] = {static_cast<float>(m_SegmentsSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);
		bgfx::setBuffer(0, m_IndirectBuffer, bgfx::Access::Write);
		bgfx::dispatch(0, m_ComputeIndirect);
    }

    void IndirectBasedLines::draw(uint viewId) const {
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
        
        bgfx::setBuffer(1, m_SegmentsBuffer, bgfx::Access::Read);

        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(viewId, m_Program, m_IndirectBuffer, 0);
    }

    void IndirectBasedLines::update(const std::vector<Segment> &segments) {
        int oldSize = m_SegmentsSize;
        m_SegmentsSize = segments.size();

        if(oldSize != m_SegmentsSize) {
            generateIndirectBuffer();
        }
         
        bgfx::update(m_SegmentsBuffer, 0, bgfx::makeRef(&segments[0], sizeof(Segment) * segments.size()));
    }
}