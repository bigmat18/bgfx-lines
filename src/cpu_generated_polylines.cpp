#include "cpu_generated_polylines.hpp"

namespace lines {
    CPUGeneratedPolylines::CPUGeneratedPolylines(const std::vector<Point> &points, const float width, const float heigth) :
        Polylines(width, heigth, "cpu_generated_polylines/vs_cpu_generated_polylines", "cpu_generated_polylines/vs_cpu_generated_polylines")
    {
        generateVertexBuffer(points);
        generateIndexBuffer(points);
    }

    CPUGeneratedPolylines::~CPUGeneratedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
    }

    void CPUGeneratedPolylines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.antialias, m_Data.thickness};
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
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }

    void CPUGeneratedPolylines::update(const std::vector<Point> &points) {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        m_Vertices.clear();
        m_Indices.clear();
        
        generateVertexBuffer(points);
        generateIndexBuffer(points);
    }

    void CPUGeneratedPolylines::generateVertexBuffer(const std::vector<Point> points) {
        std::vector<std::array<float, 4>> prev(points.size());
        std::vector<std::array<float, 4>> curr(points.size());
        std::vector<std::array<float, 4>> next(points.size());

        float length = 0;
        for(uint32_t i = 0; i < points.size(); i++) {
            length += calculateDistance(points[i-1 != -1 ? i-1 : 0], points[i]);
            std::array<float, 4> element = {points[i].x, points[i].y, points[i].z, length};

            curr[i] = element;

            if (i == 0) prev[i] = element;
            else prev[i] = {points[i - 1].x, points[i - 1].y, points[i - 1].z, 0.0f};

            if (i == points.size() - 1) next[i] = element;
            else next[i] = {points[i+1].x, points[i+1].y, points[i+1].z, 0.0f};
        }

        for(uint32_t i = 0; i < curr.size() * 2; i++) {
        
            // a_position ==> prev(x,y,z)
            m_Vertices.push_back(prev[i/2][0]);
            m_Vertices.push_back(prev[i/2][1]);
            m_Vertices.push_back(prev[i/2][2]);

            // a_texcoord0 ==> curr(x,y,z)
            m_Vertices.push_back(curr[i/2][0]);
            m_Vertices.push_back(curr[i/2][1]);
            m_Vertices.push_back(curr[i/2][2]);

            // a_texcoord1 ==> next(x,y,z)
            m_Vertices.push_back(next[i/2][0]);
            m_Vertices.push_back(next[i/2][1]);
            m_Vertices.push_back(next[i/2][2]);

            // a_texcoord2 ==> uv(up/low, distance)
            m_Vertices.push_back(i % 2 == 0 ? 1.0 : -1.0);
            m_Vertices.push_back(curr[i/2][3]);
        }

        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()),
            layout
        );
    }

    void CPUGeneratedPolylines::generateIndexBuffer(const std::vector<Point> points) {
        uint32_t j = 0;
        for(uint32_t i = 0; i < points.size() - 1; i++, j+=2) {
            m_Indices.push_back(j);
            m_Indices.push_back(j+1);
            m_Indices.push_back(j+2);

            m_Indices.push_back(j+1);
            m_Indices.push_back(j+3);
            m_Indices.push_back(j+2);
        }

        m_Ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_Indices[0], sizeof(uint32_t) * m_Indices.size()),
            BGFX_BUFFER_INDEX32
        );
    }
}