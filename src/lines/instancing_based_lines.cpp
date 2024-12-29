#include <lines/instancing_based_lines.hpp>

namespace lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "lines/instancing_based_lines/vs_instancing_based_lines", "lines/instancing_based_lines/fs_instancing_based_lines") 
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

        generateInstanceDataBuffer(segments);
    }

    InstancingBasedLines::~InstancingBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void InstancingBasedLines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.thickness, 0};
        bgfx::setUniform(m_UniformData1, data);

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
        
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedLines::update(const std::vector<Segment> &segments) {
        generateInstanceDataBuffer(segments);
    }

    void InstancingBasedLines::generateInstanceDataBuffer(const std::vector<Segment> &segments) {
        const uint16_t stride = sizeof(float) * 12;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(segments.size(), stride);
        bgfx::allocInstanceDataBuffer(&m_IDBSegments, linesNum, stride);

        uint8_t* data = m_IDBSegments.data;
        for(uint32_t i = 0; i < segments.size(); i++) {
            float* p0 = reinterpret_cast<float*>(data);
            p0[0] = segments[i].m_P0.x;
            p0[1] = segments[i].m_P0.y;
            p0[2] = segments[i].m_P0.z;
            p0[3] = 0.0f;

            float* p1 = (float*)&data[16];
            p1[0] = segments[i].m_P1.x;
            p1[1] = segments[i].m_P1.y;
            p1[2] = segments[i].m_P1.z;
            p1[3] = 0.0f;

            float* color = (float*)&data[32];
            color[0] = segments[i].m_Color.r;
            color[1] = segments[i].m_Color.g;
            color[2] = segments[i].m_Color.b;
            color[3] = segments[i].m_Color.a;

            data += stride;
        }
    }
}