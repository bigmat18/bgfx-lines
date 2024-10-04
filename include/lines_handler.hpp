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
            virtual void BeginLine() = 0;
            virtual void AddPoint(const LinesPoint &point) = 0;
            virtual void EndLine() = 0;
            virtual void Render(uint64_t state) = 0;

        protected:
            LinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name) : 
                m_RenderState(state), vs_name(vs_name), fs_name(fs_name) {}
            
            virtual ~LinesHandler() = default;

            bgfx::ProgramHandle LoadProgram(const std::string vs_name, const std::string fs_name) {
                return loadProgram(vs_name.c_str(), fs_name.c_str());
            }

            bgfx::ProgramHandle m_program;
            bgfx::VertexBufferHandle m_vbh;
            bgfx::IndexBufferHandle m_ibh;
            bgfx::VertexLayout m_layout;

            std::vector<float> m_vertices;
            std::vector<uint32_t> m_indices;

            const std::string vs_name;
            const std::string fs_name;

            std::vector<LinesPoint> m_Points;       
            uint64_t m_RenderState;     
    };
}