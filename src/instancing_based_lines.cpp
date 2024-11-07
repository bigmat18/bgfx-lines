#include <instancing_based_lines.hpp>

namespace lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "instanced_based_lines/vs_instanced_based_lines", "instanced_based_lines/fs_instanced_based_lines") 
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

        setSegments(segments);
    }


    void InstancingBasedLines::draw(uint viewId) const {
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
        bgfx::setInstanceDataBuffer(&m_IDBSegments);
        
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedLines::setSegments(const std::vector<Segment> &segments) {
        const uint16_t stride = sizeof(float) * 12;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(segments.size(), stride);
        bgfx::allocInstanceDataBuffer(&m_IDBSegments, linesNum, stride);

        uint8_t* data = m_IDBSegments.data;
        for(uint32_t i = 0; i < segments.size(); i++) {
            float* s = reinterpret_cast<float*>(data);
            s[0] = segments[i].m_P0.x;
            s[1] = segments[i].m_P0.y;
            s[2] = segments[i].m_P0.z;
            s[3] = 0.0f;

            s[4] = segments[i].m_P1.x;
            s[5] = segments[i].m_P1.y;
            s[6] = segments[i].m_P1.z;
            s[7] = 0.0f;

            s[8] = segments[i].m_Color.r;
            s[9] = segments[i].m_Color.g;
            s[10] = segments[i].m_Color.b;
            s[11] = segments[i].m_Color.a;

            data += stride;
        }
    }

}