#include <lines_handler.hpp>

namespace Lines {
    LinesHandler::LinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name, const std::string name) : 
        m_state(state), vs_name(vs_name), fs_name(fs_name), m_active(true), m_name(name), m_closed(false)
    {
        m_program = LoadProgram(vs_name, fs_name);
    }

    LinesHandler::~LinesHandler() {
        bgfx::destroy(m_ibh);
        bgfx::destroy(m_vbh),
        bgfx::destroy(m_program);
    };

    void LinesHandler::BeginLine() {
        m_Points.clear();
        m_vertices.clear();
        m_indices.clear();
    };

    void LinesHandler::AddPoint(const LinesPoint &point) {
        m_Points.push_back(point);
    }

    bgfx::ProgramHandle LinesHandler::LoadProgram(const std::string vs_name, const std::string fs_name) {
        return loadProgram(vs_name.c_str(), fs_name.c_str());
    }

    float LinesHandler::CalculateDistance(const LinesPoint &p1, const LinesPoint &p2) {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }
}