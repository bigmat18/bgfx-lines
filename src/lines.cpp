#include <lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>

namespace lines {
    TriangulatedLines::TriangulatedLines(const std::vector<LinePoint>& points, const float width, const float heigth) {
        setScreenSize(width, heigth);
        
        generateVertexBuffer(points);
        generateIndexBuffer(points);
        m_Program = vcl::loadProgram("triangulated_lines/vs_triangulated_lines", "triangulated_lines/fs_triangulated_lines");
        m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
        m_UniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    }

    TriangulatedLines::TriangulatedLines(const bgfx::VertexBufferHandle vbh) {

    }

    TriangulatedLines::TriangulatedLines(const bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh) {

    }

    TriangulatedLines::~TriangulatedLines() {
        bgfx::destroy(m_UniformColor);
        bgfx::destroy(m_UniformData);
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_Program);
    }

    void TriangulatedLines::draw(uint viewId) const {
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

    void TriangulatedLines::generateVertexBuffer(const std::vector<LinePoint> points) {

        for(uint i = 0; i < points.size() - 1; i++) {

            for(uint k = 0; k < 2; k++) {

                for(uint j = 0; j < 2; j++) {
                    int u = k % 2 == 0 ? 0 : 1;
                    int v = j % 2 == 0 ? 1 : 0;

                    vertices.push_back(points[i].x);
                    vertices.push_back(points[i].y);
                    vertices.push_back(points[i].z);

                    vertices.push_back(points[i + 1].x);
                    vertices.push_back(points[i + 1].y);
                    vertices.push_back(points[i + 1].z);

                    vertices.push_back(u);
                    vertices.push_back(v);
                }
            }
        }

        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
         .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&vertices[0], sizeof(float) * vertices.size()),
            layout
        );
    }

    void TriangulatedLines::generateIndexBuffer(const std::vector<LinePoint> points) {
        for(uint32_t i = 0; i < points.size() - 1; i++) {
            indices.push_back(i + (3 * i));
            indices.push_back(i + (3 * i) + 1);
            indices.push_back(i + (3 * i) + 2);

            indices.push_back(i + (3 * i) + 1);
            indices.push_back(i + (3 * i) + 3);
            indices.push_back(i + (3 * i) + 2);
        }

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&indices[0], sizeof(uint32_t) * indices.size()),
            BGFX_BUFFER_INDEX32
        );
    }
}