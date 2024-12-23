#include <lines/texture_based_lines.hpp>
#include <vclib/bgfx/context/load_program.h>
#include <assert.h>

namespace lines {
    TextureBasedLines::TextureBasedLines(const std::vector<Segment> &segments, const float width, const float heigth, const uint32_t maxTextureSize) :
        Lines(width, heigth, "lines/texture_based_lines/vs_texture_based_lines", "lines/texture_based_lines/fs_texture_based_lines"),
        m_SegmentsSize(segments.size()),
        m_MaxTextureSize(maxTextureSize)
    {
        assert(m_SegmentsSize <= (maxTextureSize * maxTextureSize) / 3);
        m_IndirectBuffer = bgfx::createIndirectBuffer(1);
        m_IndirectDataUniform = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);
        m_ComputeIndirect = bgfx::createProgram(vcl::loadShader("lines/texture_based_lines/cs_compute_indirect"), true);
        m_ComputeTexture = bgfx::createProgram(vcl::loadShader("lines/texture_based_lines/cs_compute_texture"), true);

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

        allocateTextureBuffer();
        generateTextureBuffer();
    }

    TextureBasedLines::~TextureBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_IndirectBuffer);
        bgfx::destroy(m_TextureBuffer);
        bgfx::destroy(m_ComputeIndirect);
        bgfx::destroy(m_IndirectDataUniform);
    }

    void TextureBasedLines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.antialias, m_Data.thickness};
        bgfx::setUniform(m_UniformData, data);

        float indirectData[] = {static_cast<float>(m_MaxTextureSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, indirectData);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        bgfx::setImage(0, m_TextureBuffer, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program, m_IndirectBuffer, 0);
    }

    void TextureBasedLines::update(const std::vector<Segment> &segments) {}

    void TextureBasedLines::generateIndirectBuffer() {
        float data[] = {static_cast<float>(m_SegmentsSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);
		bgfx::setBuffer(0, m_IndirectBuffer, bgfx::Access::Write);
		bgfx::dispatch(0, m_ComputeIndirect);
    }

    void TextureBasedLines::generateTextureBuffer() {
        float data[] = {static_cast<float>(m_MaxTextureSize), 0, 0, 0};
        bgfx::setUniform(m_IndirectDataUniform, data);

        bgfx::setBuffer(0, m_SegmentsBuffer, bgfx::Access::Read);
        bgfx::setImage(1, m_TextureBuffer, 0, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeTexture, m_SegmentsSize, 1, 1);
    }

    void TextureBasedLines::allocateTextureBuffer() {
        uint16_t Y = (m_SegmentsSize * 3) / (m_MaxTextureSize + 1);
        uint16_t X = Y == 0 ? (m_SegmentsSize * 3) : m_MaxTextureSize;
        
        m_TextureBuffer = bgfx::createTexture2D(
            X, Y + 1, false, 1, bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE
        );
    }

    void TextureBasedLines::allocateSegmentsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_SegmentsBuffer = bgfx::createDynamicVertexBuffer(
            m_SegmentsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

}