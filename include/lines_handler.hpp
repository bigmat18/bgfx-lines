#pragma once
#include <bgfx_utils.h>
#include <vector>
#include <string>

namespace Lines {

    struct LinesPoint{
        float x;
        float y;
        float z;

        LinesPoint(float xx, float yy, float zz) :
            x(xx), y(yy), z(zz) {}
    };

    class LinesHandler {
        friend class LinesFactory;

        public:
            void BeginLine();
            void AddPoint(const LinesPoint &point);
            
            virtual void EndLine() = 0;
            virtual void Render(uint64_t state) = 0;

            void SetRenderActive(bool active) { m_renderActive = active; }
            std::string GetName() const { return m_name; }

        protected:
            LinesHandler(uint64_t state,
                         const std::string vs_name,
                         const std::string fs_name,
                         const std::string name);

            virtual ~LinesHandler();

            bgfx::ProgramHandle LoadProgram(const std::string vs_name, const std::string fs_name);

            bgfx::ProgramHandle m_program;
            bgfx::VertexBufferHandle m_vbh;
            bgfx::IndexBufferHandle m_ibh;
            bgfx::VertexLayout m_layout;

            std::vector<float> m_vertices;
            std::vector<uint32_t> m_indices;

            const std::string vs_name;
            const std::string fs_name;

            std::vector<LinesPoint> m_Points;

            uint64_t m_renderState;
            bool m_renderActive;
            std::string m_name;
    };
}