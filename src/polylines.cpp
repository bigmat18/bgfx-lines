#include <vclib/render_bgfx/context/load_program.h>
#include <cpu_generated_polylines.hpp>
#include <polylines.hpp>

namespace lines {

    std::unique_ptr<Polylines> Polylines::create(const std::vector<Point> &points, const float width, const float heigth, Types type) {
        const bgfx::Caps* caps = bgfx::getCaps();
        
        switch (type) {
            case Types::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedPolylines>(points, width, heigth);
            }

            case Types::GPU_GENERATED: {
                bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return nullptr;
            }

            case Types::INSTANCING_BASED: {
                const bool instancingSupported = 0 != (BGFX_CAPS_INSTANCING & caps->supported);
                assert((void("Instancing not supported"), instancingSupported));
                return nullptr;
            }
        }
        assert((void("Lines type is incorrect"), true));
        return nullptr;
    }

    std::unique_ptr<Polylines> Polylines::create(bgfx::VertexBufferHandle vbh) {
        /** ... */
        return nullptr;
    }

    std::unique_ptr<Polylines> Polylines::create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh) {
        /** ... */
        return nullptr;
    }

    Polylines::Polylines(const float width, const float heigth, const std::string& vs_name,  const std::string& fs_name) {
        setScreenSize(width, heigth);

        m_Program = vcl::loadProgram(vs_name, fs_name);
        m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
        m_UniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    }

    Polylines::~Polylines() {
        bgfx::destroy(m_UniformData);
        bgfx::destroy(m_UniformColor);
        bgfx::destroy(m_Program);
    }
}