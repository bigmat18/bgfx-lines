#include <lines.hpp>
#include <cpu_generated_lines.hpp>
#include <instancing_based_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>

namespace lines {

    std::unique_ptr<Lines> Lines::create(const std::vector<LinesPoint> &points, const float width, const float heigth, LinesType type) {
        switch (type) {
            case LinesType::CPU_GENERATED_LINES: {
                return std::make_unique<CPUGeneratedLines>(points, width, heigth);
            }

            case LinesType::GPU_GENERATED_LINES: {
                return nullptr;
            }

            case LinesType::INSTANCING_BASED_LINES: {
                const bgfx::Caps* caps = bgfx::getCaps();

                const bool instancingSupported = 0 != (BGFX_CAPS_INSTANCING & caps->supported);
                assert((void("Instancing not supported"), instancingSupported));
                
                return std::make_unique<InstancingBasedLines>(points, width, heigth);
            }
        }
        assert((void("Lines type is incorrect"), true));
        return nullptr;
    }

    std::unique_ptr<Lines> Lines::create(bgfx::VertexBufferHandle vbh) {
        /** ... */
    }

    std::unique_ptr<Lines> Lines::create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh) {
        /** ... */
    }

    Lines::Lines(const float width, const float heigth, const std::string& vs_name,  const std::string& fs_name) {
        setScreenSize(width, heigth);

        m_Program = vcl::loadProgram(vs_name, fs_name);
        m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
        m_UniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    }

    Lines::~Lines() {
        bgfx::destroy(m_UniformColor);
        bgfx::destroy(m_UniformData);
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_Ibh);
        bgfx::destroy(m_Program);
    }
}