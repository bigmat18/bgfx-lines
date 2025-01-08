#include <lines.hpp>
#include <lines/cpu_generated_lines.hpp>
#include <lines/gpu_generated_lines.hpp>
#include <lines/instancing_based_lines.hpp>
#include <lines/indirect_based_lines.hpp>
#include <lines/texture_based_lines.hpp>

#include <vclib/bgfx/context/load_program.h>

namespace lines {

    std::unique_ptr<Lines> Lines::create(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth, Types type) {
        const bgfx::Caps* caps = bgfx::getCaps();

        switch (type) {
            case Types::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedLines>(points, width, heigth);
            }

            case Types::GPU_GENERATED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return std::make_unique<GPUGeneratedLines>(points, width, heigth);
            }

            case Types::INSTANCING_BASED: {
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                assert((void("Instancing not supported"), instancingSupported));
                return std::make_unique<InstancingBasedLines>(points, width, heigth);
            }

            case Types::INDIRECT_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);

                assert((void("Instancing or compute are not supported"), instancingSupported && computeSupported && indirectSupported));
                return std::make_unique<IndirectBasedLines>(points, width, heigth);
            }

            case Types::TEXTURE_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                const bool textureSupported = !!(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

                assert((void("Instancing or compute or indirect or texture are not supported"), instancingSupported && computeSupported && indirectSupported && textureSupported));
                return std::make_unique<TextureBasedLines>(points, width, heigth, caps->limits.maxTextureSize);
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

    Lines::Lines(const uint16_t width, const uint16_t heigth, const std::string& vs_name,  const std::string& fs_name) {
        m_Settings = LinesSettings(width, heigth);
        m_Program = vcl::loadProgram(vs_name, fs_name);
    }

    Lines::~Lines() {
        bgfx::destroy(m_Program);
    } 
}