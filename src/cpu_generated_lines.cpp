#include <cpu_generated_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>


namespace lines {
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth) :
        Lines(width, heigth, "cpu_generated_lines/vs_cpu_generated_lines", "cpu_generated_lines/fs_cpu_generated_lines"),
        m_SegmentsSize(segments.size())
    {
        allocateVertexBuffer();
        allocateIndexBuffer();
        generateBuffers(segments);
    }

    CPUGeneratedLines::~CPUGeneratedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void CPUGeneratedLines::update(const std::vector<Segment> &segments) {
        m_Vertices.clear();
        m_Indices.clear();

        if(m_SegmentsSize > segments.size()) {
            bgfx::destroy(m_Vbh);
            bgfx::destroy(m_Ibh);

            allocateVertexBuffer();
            allocateIndexBuffer();
        }

        m_SegmentsSize = segments.size();
        generateBuffers(segments);
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

    void CPUGeneratedLines::generateBuffers(const std::vector<Segment> segments) {
        for(uint i = 0; i < segments.size(); i++) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {
                    int u = k == 0 ? 1 : -1;
                    int v = j == 0 ? 1 : -1;

                    Point p0 = k == 0 ? segments[i].m_P0 : segments[i].m_P1;
                    Point p1 = k == 0 ? segments[i].m_P1 : segments[i].m_P0;

                    m_Vertices.push_back(p0.x);
                    m_Vertices.push_back(p0.y);
                    m_Vertices.push_back(p0.z);
                    
                    m_Vertices.push_back(p1.x);
                    m_Vertices.push_back(p1.y);
                    m_Vertices.push_back(p1.z);

                    m_Vertices.push_back(segments[i].m_Color.r);
                    m_Vertices.push_back(segments[i].m_Color.g);
                    m_Vertices.push_back(segments[i].m_Color.b);
                    m_Vertices.push_back(segments[i].m_Color.a);

                    m_Vertices.push_back(v * u);
                }
            }
            m_Indices.push_back((4 * i));
            m_Indices.push_back((4 * i) + 1);
            m_Indices.push_back((4 * i) + 2);

            m_Indices.push_back((4 * i) + 1);
            m_Indices.push_back((4 * i) + 3);
            m_Indices.push_back((4 * i) + 2);
        }

        bgfx::update(m_Vbh, 0, bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()));
        bgfx::update(m_Ibh, 0, bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()));
    }

    void CPUGeneratedLines::allocateVertexBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 1, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createDynamicVertexBuffer(m_SegmentsSize * 4, layout, BGFX_BUFFER_ALLOW_RESIZE);
    }

    void CPUGeneratedLines::allocateIndexBuffer() {
        m_Ibh = bgfx::createDynamicIndexBuffer(m_SegmentsSize * 6, BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }
}