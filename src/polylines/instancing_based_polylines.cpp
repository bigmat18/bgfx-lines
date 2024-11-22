#include <polylines/instancing_based_polylines.hpp>
#include <cmath>

namespace lines {
    InstancingBasedPolylines::InstancingBasedPolylines(const std::vector<Point> &points, const float width, const float heigth) :
        Polylines(width, heigth, "polylines/instancing_based_polylines/vs_instancing_based_polylines", "polylines/instancing_based_polylines/fs_instancing_based_polylines")

    {
        m_Vertices = {
            0.0f, 0.0f,
            0.0f, 1.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f,
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
            0, 3, 1,
            0, 2, 3,
        };

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()),
            BGFX_BUFFER_INDEX32
        );

        generateInstanceDataBuffer(points);
    }

    InstancingBasedPolylines::~InstancingBasedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void InstancingBasedPolylines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.miterLimit, m_Data.thickness};
        bgfx::setUniform(m_UniformData, data);
        bgfx::setUniform(m_UniformColor, &m_Data.color);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_Ibh);
        bgfx::setInstanceDataBuffer(&m_InstanceDataBuffer);

        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedPolylines::update(const std::vector<Point> &points) {
        generateInstanceDataBuffer(points);
    }

    void InstancingBasedPolylines::generateInstanceDataBuffer(const std::vector<Point> &points) {
        const uint16_t stride = sizeof(float) * 16;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer(points.size() - 1, stride);
        bgfx::allocInstanceDataBuffer(&m_InstanceDataBuffer, linesNum, stride);

        uint8_t* data = m_InstanceDataBuffer.data;
        for(uint32_t i = 0; i < linesNum; i++) {
            float* prev = reinterpret_cast<float*>(data);
            prev[0] = points[i - !!i].x;
            prev[1] = points[i - !!i].y;
            prev[2] = points[i - !!i].z;
            prev[3] = 0;

            float* curr = (float*)&data[16];
            curr[0] = points[i].x;
            curr[1] = points[i].y;
            curr[2] = points[i].z;
            curr[3] = 0;

            float* next = (float*)&data[32];
            next[0] = points[i + !!(linesNum - 1 - i)].x;
            next[1] = points[i + !!(linesNum - 1 - i)].y;
            next[2] = points[i + !!(linesNum - 1 - i)].z;
            next[3] = 0;

            float* next_next = (float*)&data[48];
            next_next[0] = points[i + (2 * !!(linesNum - 1 - i))].x;
            next_next[1] = points[i + (2 * !!(linesNum - 1 - i))].y;
            next_next[2] = points[i + (2 * !!(linesNum - 1 - i))].z;
            next_next[3] = 0;
        }
    }
}