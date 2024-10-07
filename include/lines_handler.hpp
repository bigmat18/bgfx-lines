#pragma once
#include <bgfx_utils.h>
#include <vector>
#include <string>

#include "lines.hpp"

namespace Lines
{

    class LinesHandler
    {
        friend LinesHandler *CreateHandler(LinesType type, const std::string name);
        friend void DestroyHandler(LinesHandler *handler);
        friend void DebugMenu();

    public:
        void BeginLine();
        void AddPoint(const LinesPoint &point);

        virtual void EndLine() = 0;
        virtual void Render(uint64_t state) = 0;

        void SetClosed(bool closed) { m_closed = closed; }
        bool isClosed() const { return m_closed; }

        void SetActive(bool active) { m_active = active; }
        bool IsActive() const { return m_active; }

        std::string GetName() const { return m_name; }
        uint64_t GetState() const { return m_state;}

        virtual ~LinesHandler();

    protected:
        LinesHandler(uint64_t state,
                     const std::string vs_name,
                     const std::string fs_name,
                     const std::string name);

        bgfx::ProgramHandle LoadProgram(const std::string vs_name, const std::string fs_name);
        float CalculateDistance(const LinesPoint &p1, const LinesPoint &p2);

        bgfx::ProgramHandle m_program;
        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;
        bgfx::VertexLayout m_layout;

        std::vector<float> m_vertices;
        std::vector<uint32_t> m_indices;

        const std::string vs_name;
        const std::string fs_name;

        std::vector<LinesPoint> m_Points;

        uint64_t m_state;
        bool m_active;
        std::string m_name;
        bool m_closed;
    };
}