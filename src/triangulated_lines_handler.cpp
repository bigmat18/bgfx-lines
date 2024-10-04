#include <triangulated_lines_handler.hpp>

namespace Lines {

    TriangulatedLinesHandler::TriangulatedLinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name) : 
        LinesHandler(state, vs_name, fs_name) 
    {
        m_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        m_program = LoadProgram(vs_name, fs_name);

        m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
        m_UniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
        m_UniformLength = bgfx::createUniform("u_length", bgfx::UniformType::Vec4);

    }

    TriangulatedLinesHandler::~TriangulatedLinesHandler() {
        bgfx::destroy(m_ibh);
        bgfx::destroy(m_vbh),
        bgfx::destroy(m_program);
        bgfx::destroy(m_UniformData);
        bgfx::destroy(m_UniformColor);
        bgfx::destroy(m_UniformLength);
    }

    void TriangulatedLinesHandler::BeginLine() {
        m_Points.clear();
    }

    void TriangulatedLinesHandler::AddPoint(const LinesPoint &point) {
        m_Points.push_back(point);
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

        for(uint32_t i = 0; i < m_Points.size(); i+=2) {
            m_indices.push_back(i);
            m_indices.push_back(i+1);
            m_indices.push_back(i+2);

            m_indices.push_back(i+1);
            m_indices.push_back(i+3);
            m_indices.push_back(i+2);
        }

        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_vertices[0], sizeof(float) * m_vertices.size()),
            m_layout
        );

        m_ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_indices[0], sizeof(uint32_t) * m_indices.size()),
            BGFX_BUFFER_INDEX32
        );


        #if DEBUGGING
            for(int i = 0; i < m_vertices.size(); i+=11) {
                std::cout << "Vertex buffer element " << i / 11 << std::endl;
                std::cout << "Prev coord: " << m_vertices[i]     << " " << m_vertices[i + 1] << " " << m_vertices[i + 2] << std::endl;
                std::cout << "Curr coord: " << m_vertices[i + 3] << " " << m_vertices[i + 4] << " " << m_vertices[i + 5] << std::endl;
                std::cout << "Next coord: " << m_vertices[i + 6] << " " << m_vertices[i + 7] << " " << m_vertices[i + 8] << std::endl;
                std::cout << "UV coord: "   << m_vertices[i + 9] << " " << m_vertices[i + 10] << " " << std::endl << std::endl;
            }
                
            for(int i = 0; i < m_indices.size(); i+=3)
                std::cout << "Triangle " << i << " : " << m_indices[i] << " " << m_indices[i+1] << " " << m_indices[i+2] << std::endl;

            std::cout << std::endl << "Line length: " << m_UniformData.length << std::endl;
            std::cout << "Thickness: " << m_UniformData.thickness << std::endl;
            std::cout << "Antialias: " << m_UniformData.antialias << std::endl;
            std::cout << "Screen size: (" << m_UniformData.resolution[0] << ", " << m_UniformData.resolution[1] << ")" << std::endl;
        #endif
    }

    void TriangulatedLinesHandler::Render(uint64_t state) {
        float data[] = {m_data.resolution[0], m_data.resolution[1], m_data.antialias, m_data.thickness};
        bgfx::setUniform(m_UniformData, data);
        bgfx::setUniform(m_UniformColor, m_data.color);

        float dataLength[] = {m_data.length, 0.0, 0.0, 0.0};
        bgfx::setUniform(m_UniformLength, dataLength);

        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(state |= this->m_RenderState);
        bgfx::submit(0, m_program);
    }

    float TriangulatedLinesHandler::CalculateDistance(const LinesPoint &p1, const LinesPoint &p2) {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }
}