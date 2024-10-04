#include <primitive_lines_handler.hpp>
#include <iostream>

namespace Lines {
    PrimitiveLinesHandler::PrimitiveLinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name) : 
        LinesHandler(state, vs_name, fs_name)
    {
        m_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        m_UniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    }

    void PrimitiveLinesHandler::EndLine() {
        for(uint32_t i = 0; i < m_Points.size(); i++) {
            m_vertices.push_back(m_Points[i].x);
            m_vertices.push_back(m_Points[i].y);
            m_vertices.push_back(m_Points[i].x);
        }

        for(uint32_t i = 0; i < m_Points.size(); i+=2) {
            m_indices.push_back(i);
            m_indices.push_back(i+1);
        }

        m_vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_vertices[0], sizeof(float) * m_vertices.size()),
            m_layout
        );

        m_ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_indices[0], sizeof(uint32_t) * m_indices.size()),
            BGFX_BUFFER_INDEX32
        );

        #if 1
            for(int i = 0; i < m_vertices.size(); i+=3) {
                std::cout << "Vertex buffer element " << i / 11 << std::endl;
                std::cout << "Coord: " << m_vertices[i]     << " " << m_vertices[i + 1] << " " << m_vertices[i + 2] << std::endl;
            }
                
            for(int i = 0; i < m_indices.size(); i+=2)
                std::cout << "Line " << i << " : " << m_indices[i] << " " << m_indices[i+1] << std::endl;
        #endif
    }

    void PrimitiveLinesHandler::Render(uint64_t state) {
        bgfx::setUniform(m_UniformColor, m_color);
        
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(state |= this->m_RenderState);
        bgfx::submit(0, m_program);
    }
}