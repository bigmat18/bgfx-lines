#include <lines.hpp>
#include <cpu_generated_lines.hpp>
#include <gpu_generated_lines.hpp>
#include <instancing_cpu_lines.hpp>
#include <instancing_gpu_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>

namespace lines {

    std::unique_ptr<Lines> Lines::create(const std::vector<Segment> &segments, const float width, const float heigth, Types type) {
        const bgfx::Caps* caps = bgfx::getCaps();
        
        switch (type) {
            case Types::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedLines>(segments, width, heigth);
            }

            case Types::GPU_GENERATED: {
                bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return std::make_unique<GPUGeneratedLines>(segments, width, heigth);
            }

            case Types::INSTANCING_CPU_GENERATED: {
                const bool instancingSupported = 0 != (BGFX_CAPS_INSTANCING & caps->supported);
                assert((void("Instancing not supported"), instancingSupported));
                return std::make_unique<InstancingCPULines>(segments, width, heigth);
            }

            case Types::INSTANCING_GPU_GENERATED: {
                bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool instancingSupported = 0 != (BGFX_CAPS_INSTANCING & caps->supported);
                assert((void("Instancing or compute are not supported"), instancingSupported || computeSupported));
                return std::make_unique<InstancingGPULines>(segments, width, heigth);
            }
        }
        assert((void("Lines type is incorrect"), true));
        return nullptr;
    }

    std::unique_ptr<Lines> Lines::create(bgfx::VertexBufferHandle vbh) {
        /** ... */
        return nullptr;
    }

    std::unique_ptr<Lines> Lines::create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh) {
        /** ... */
        return nullptr;
    }

    Lines::Lines(const float width, const float heigth, const std::string& vs_name,  const std::string& fs_name) {
        setScreenSize(width, heigth);

        m_Program = vcl::loadProgram(vs_name, fs_name);
        m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
    }

    Lines::~Lines() {
        bgfx::destroy(m_UniformData);
        bgfx::destroy(m_Program);
    }
}