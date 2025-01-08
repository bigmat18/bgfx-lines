#include <lines/instancing_based_lines.hpp>

namespace lines {
    InstancingBasedLines::InstancingBasedLines(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth) :
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

        generateInstanceDataBuffer(points);
    }

    InstancingBasedLines::~InstancingBasedLines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void InstancingBasedLines::draw(uint viewId) const {
        m_Settings.bindUniformLines();

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
        
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void InstancingBasedLines::update(const std::vector<Point> &points) {
        generateInstanceDataBuffer(points);
    }

    void InstancingBasedLines::generateInstanceDataBuffer(const std::vector<Point> &points) {
        const uint16_t stride = sizeof(float) * 8;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer((points.size() / 2), stride);
        bgfx::allocInstanceDataBuffer(&m_IDBPoints, linesNum, stride);

        uint8_t* data = m_IDBPoints.data;
        for(uint32_t i = 1; i < points.size(); i+=2) {
            uint32_t color0_hex = static_cast<uint32_t>(std::round(points[i-1].color.r * 255)) << 24 |
                                  static_cast<uint32_t>(std::round(points[i-1].color.g * 255)) << 16 |
                                  static_cast<uint32_t>(std::round(points[i-1].color.b * 255)) << 8  |
                                  static_cast<uint32_t>(std::round(points[i-1].color.a * 255));

            uint32_t color1_hex = static_cast<uint32_t>(std::round(points[i].color.r * 255)) << 24 |
                                  static_cast<uint32_t>(std::round(points[i].color.g * 255)) << 16 |
                                  static_cast<uint32_t>(std::round(points[i].color.b * 255)) << 8  |
                                  static_cast<uint32_t>(std::round(points[i].color.a * 255));

            float* p0 = reinterpret_cast<float*>(data);
            p0[0] = points[i-1].x;
            p0[1] = points[i-1].y;
            p0[2] = points[i-1].z;

            uint32_t* color0 = (uint32_t*)&data[12];
            color0[0] = color0_hex;

            float* p1 = (float*)&data[16];
            p1[0] = points[i].x;
            p1[1] = points[i].y;
            p1[2] = points[i].z;

            uint32_t* color1 = (uint32_t*)&data[28];
            color1[0] = color1_hex;

            data += stride;
        }
    }
}