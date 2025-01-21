#include <lines.hpp>
#include <lines/primitive_lines.hpp>
#include <lines/cpu_generated_lines.hpp>
#include <lines/gpu_generated_lines.hpp>
#include <lines/instancing_based_lines.hpp>
#include <lines/indirect_based_lines.hpp>
#include <lines/texture_based_lines.hpp>

#include <vclib/bgfx/context/load_program.h>

namespace lines {

    std::unique_ptr<Lines> Lines::create(const std::vector<LinesVertex> &points, LinesTypes type) {
        const bgfx::Caps* caps = bgfx::getCaps();

        switch (type) {
            case LinesTypes::PRIMITIVE: {
                return std::make_unique<PrimitiveLines>(points);
            }

            case LinesTypes::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedLines>(points);
            }

            case LinesTypes::GPU_GENERATED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return std::make_unique<GPUGeneratedLines>(points);
            }

            case LinesTypes::INSTANCING_BASED: {
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                assert((void("Instancing not supported"), instancingSupported));
                return std::make_unique<InstancingBasedLines>(points);
            }

            case LinesTypes::INDIRECT_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);

                assert((void("Instancing or compute are not supported"), instancingSupported && computeSupported && indirectSupported));
                return std::make_unique<IndirectBasedLines>(points);
            }

            case LinesTypes::TEXTURE_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                const bool textureSupported = !!(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

                assert((void("Instancing or compute or indirect or texture are not supported"), instancingSupported && computeSupported && indirectSupported && textureSupported));
                return std::make_unique<TextureBasedLines>(points, caps->limits.maxTextureSize);
            }
        }

        assert((void("Lines type is incorrect"), true));
        throw std::invalid_argument("Invalid enum case");
    }

    Lines::Lines(const std::string& vs_name,  const std::string& fs_name) : 
        vs_name(vs_name), fs_name(fs_name)
    {
        mLinesPH = vcl::loadProgram(vs_name, fs_name);
        assert(bgfx::isValid(mLinesPH));
    }

    Lines::Lines(const Lines& other) {
        vs_name = other.vs_name;
        fs_name = other.fs_name;

        mLinesPH = vcl::loadProgram(vs_name, fs_name);
        mSettings = other.mSettings;
        assert(bgfx::isValid(mLinesPH));
    }

    Lines::Lines(Lines&& other) {
        vs_name = other.vs_name;
        fs_name = other.fs_name;

        mLinesPH = vcl::loadProgram(vs_name, fs_name);
        mSettings = other.mSettings;
        assert(bgfx::isValid(mLinesPH));
    }

    Lines::~Lines() {
        if(bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    } 
}