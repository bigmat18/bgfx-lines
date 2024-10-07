#include <triangulated_lines_handler.hpp>

namespace Lines {

    TriangulatedLinesHandler::TriangulatedLinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name, const std::string name) : 
        LinesHandler(state, vs_name, fs_name, name)
    {
        m_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
        m_UniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
        m_UniformLength = bgfx::createUniform("u_length", bgfx::UniformType::Vec4);

    }

    TriangulatedLinesHandler::~TriangulatedLinesHandler() {
        bgfx::destroy(m_UniformData);
        bgfx::destroy(m_UniformColor);
        bgfx::destroy(m_UniformLength);
    }

    void TriangulatedLinesHandler::EndLine() {
        std::vector<std::array<float, 4>> prev(m_Points.size());
        std::vector<std::array<float, 4>> curr(m_Points.size());
        std::vector<std::array<float, 4>> next(m_Points.size());

        for(uint32_t i = 0; i < m_Points.size(); i++) {
            m_data.length += CalculateDistance(m_Points[i-1 != -1 ? i-1 : 0], m_Points[i]);
            std::array<float, 4> element = {m_Points[i].x, m_Points[i].y, m_Points[i].z, m_data.length};

            curr[i] = element;

            if (i == 0) prev[i] = element;
            else prev[i] = {m_Points[i - 1].x, m_Points[i - 1].y, m_Points[i - 1].z, 0.0f};

            if (i == m_Points.size() - 1) next[i] = element;
            else next[i] = {m_Points[i+1].x, m_Points[i+1].y, m_Points[i+1].z, 0.0f};
        }

        for(uint32_t i = 0; i < curr.size() * 2; i++) {
        
            // a_position ==> prev(x,y,z)
            m_vertices.push_back(prev[i/2][0]);
            m_vertices.push_back(prev[i/2][1]);
            m_vertices.push_back(prev[i/2][2]);

            // a_texcoord0 ==> curr(x,y,z)
            m_vertices.push_back(curr[i/2][0]);
            m_vertices.push_back(curr[i/2][1]);
            m_vertices.push_back(curr[i/2][2]);

            // a_texcoord1 ==> next(x,y,z)
            m_vertices.push_back(next[i/2][0]);
            m_vertices.push_back(next[i/2][1]);
            m_vertices.push_back(next[i/2][2]);

            // a_texcoord2 ==> uv(up/low, distance)
            m_vertices.push_back(i % 2 == 0 ? 1.0 : -1.0);
            m_vertices.push_back(curr[i/2][3]);
        }

        for(uint32_t i = 0, j = 0; i < m_Points.size(); i++, j+=2) {
            m_indices.push_back(j);
            m_indices.push_back(j+1);
            m_indices.push_back(j+2);

            m_indices.push_back(j+1);
            m_indices.push_back(j+3);
            m_indices.push_back(j+2);
        }

        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_vertices[0], sizeof(float) * m_vertices.size()),
            m_layout
        );

        m_ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_indices[0], sizeof(uint32_t) * m_indices.size()),
            BGFX_BUFFER_INDEX32
        );
    }

    void TriangulatedLinesHandler::Render(uint64_t state) {
        float data[] = {m_data.resolution[0], m_data.resolution[1], m_data.antialias, m_data.thickness};
        bgfx::setUniform(m_UniformData, data);
        bgfx::setUniform(m_UniformColor, m_data.color);

        float dataLength[] = {m_data.length, 0.0, 0.0, 0.0};
        bgfx::setUniform(m_UniformLength, dataLength);

        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(state |= m_state);
        bgfx::submit(0, m_program);
    }
}