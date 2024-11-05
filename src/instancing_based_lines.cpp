#include <instancing_based_lines.hpp>

namespace lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<LinesPoint> &points, const float width, const float heigth) :
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

        setPoints(points);
    }


    void InstancingBasedLines::draw(uint viewId) const {
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
        bgfx::setInstanceDataBuffer(&m_IDBPoints);
        
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedLines::setPoints(const std::vector<LinesPoint> &points) {
        const uint16_t stride = sizeof(float) * 8;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(points.size() / 2.0, stride);
        bgfx::allocInstanceDataBuffer(&m_IDBPoints, linesNum, stride);

        uint8_t* data = m_IDBPoints.data;
        for(uint32_t i = 0; i < points.size(); i++) {
            float* p = reinterpret_cast<float*>(data);
            p[0] = points[i].x;
            p[1] = points[i].y;
            p[2] = points[i].z;
            p[3] = 0.0f;

            data += (stride / 2);
        }
    }

}