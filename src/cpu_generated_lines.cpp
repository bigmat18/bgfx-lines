#include <cpu_generated_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>


namespace lines {
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "cpu_generated_lines/vs_cpu_generated_lines", "cpu_generated_lines/fs_cpu_generated_lines") {
        
        generateVertexBuffer(segments);
        generateIndexBuffer(segments);
    }

    void CPUGeneratedLines::draw(uint viewId) const {
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
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void CPUGeneratedLines::generateVertexBuffer(const std::vector<Segment> segments) {
        for(uint i = 0; i < segments.size(); i++) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {
                    int u = k % 2 == 0 ? 0 : 1;
                    int v = j % 2 == 0 ? 1 : 0;

                    m_Vertices.push_back(segments[i].m_P0.x);
                    m_Vertices.push_back(segments[i].m_P0.y);
                    m_Vertices.push_back(segments[i].m_P0.z);

                    m_Vertices.push_back(segments[i].m_Color.r);
                    m_Vertices.push_back(segments[i].m_Color.g);
                    m_Vertices.push_back(segments[i].m_Color.b);
                    m_Vertices.push_back(segments[i].m_Color.a);

                    m_Vertices.push_back(segments[i].m_P1.x);
                    m_Vertices.push_back(segments[i].m_P1.y);
                    m_Vertices.push_back(segments[i].m_P1.z);

                    m_Vertices.push_back(u);
                    m_Vertices.push_back(v);
                }
            }
        }

        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()),
            layout
        );
    }


    void CPUGeneratedLines::generateIndexBuffer(const std::vector<Segment> segments) {
        for(uint32_t i = 0; i < segments.size(); i++) {
            m_Indices.push_back(i + (3 * i));
            m_Indices.push_back(i + (3 * i) + 1);
            m_Indices.push_back(i + (3 * i) + 2);

            m_Indices.push_back(i + (3 * i) + 1);
            m_Indices.push_back(i + (3 * i) + 3);
            m_Indices.push_back(i + (3 * i) + 2);
        }

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()),
            BGFX_BUFFER_INDEX32
        );
    }
}