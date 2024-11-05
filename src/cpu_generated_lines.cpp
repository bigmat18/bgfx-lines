#include <cpu_generated_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>


namespace lines {
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesPoint> &points, const float width, const float heigth) :
        Lines(width, heigth, "cpu_generated_lines/vs_cpu_generated_lines", "cpu_generated_lines/fs_cpu_generated_lines") {
        
        generateVertexBuffer(points);
        generateIndexBuffer(points);
    }

    void CPUGeneratedLines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.antialias, m_Data.thickness};
        bgfx::setUniform(m_UniformData, data);
        bgfx::setUniform(m_UniformColor, m_Data.color);

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

    void CPUGeneratedLines::generateVertexBuffer(const std::vector<LinesPoint> points) {
        uint i = 0;
        for(; i < points.size() - 1; i++) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {
                    int u = k % 2 == 0 ? 0 : 1;
                    int v = j % 2 == 0 ? 1 : 0;

                    m_Vertices.push_back(points[i].x);
                    m_Vertices.push_back(points[i].y);
                    m_Vertices.push_back(points[i].z);

                    m_Vertices.push_back(points[i + 1].x);
                    m_Vertices.push_back(points[i + 1].y);
                    m_Vertices.push_back(points[i + 1].z);

                    m_Vertices.push_back(u);
                    m_Vertices.push_back(v);
                }
            }
            m_BoundingBox.add(vcl::Point3d(points[i].x, points[i].y, points[i].z));
        }
        m_BoundingBox.add(vcl::Point3d(points[i].x, points[i].y, points[i].z));

        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()),
            layout
        );
    }


    void CPUGeneratedLines::generateIndexBuffer(const std::vector<LinesPoint> points) {
        for(uint32_t i = 0; i < points.size() - 1; i++) {
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