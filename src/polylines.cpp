#include <polylines.hpp>
#include <polylines/cpu_generated_polylines.hpp>
#include <polylines/gpu_generated_polylines.hpp>
#include <polylines/instancing_based_polylines.hpp>
#include <polylines/indirect_based_polylines.hpp>
#include <polylines/texture_based_polylines.hpp>

namespace lines {

    std::unique_ptr<Polylines> Polylines::create(const std::vector<LinesVertex> &points, LinesTypes type) {
        const bgfx::Caps* caps = bgfx::getCaps();
        
        switch (type) {
            case LinesTypes::CPU_GENERATED: {
                return std::make_unique<CPUGeneratedPolylines>(points);
            }

            case LinesTypes::GPU_GENERATED: {
                bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                assert((void("GPU compute not supported"), computeSupported));
                return std::make_unique<GPUGeneratedPolylines>(points);
            }

            case LinesTypes::INSTANCING_BASED: {
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                assert((void("Instancing not supported"), instancingSupported));
                return std::make_unique<InstancingBasedPolylines>(points);
            }

            case LinesTypes::INDIRECT_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);

                assert((void("Instancing or compute are not supported"), instancingSupported && computeSupported && indirectSupported));
                return std::make_unique<IndirectBasedPolylines>(points);
            }
            case LinesTypes::TEXTURE_BASED: {
                const bool computeSupported  = !!(caps->supported & BGFX_CAPS_COMPUTE);
                const bool indirectSupported = !!(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
                const bool instancingSupported = !!(caps->supported & BGFX_CAPS_INSTANCING);
                const bool textureSupported = !!(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

                assert((void("Instancing or compute or indirect or texture are not supported"), instancingSupported && computeSupported && indirectSupported && textureSupported));
                return std::make_unique<TextureBasedPolylines>(points, caps->limits.maxTextureSize);
            }
        }
        assert((void("Lines type is incorrect"), true));
        throw std::invalid_argument("Invalid enum case");
    }

    Polylines::Polylines(const std::string& vs_name,  const std::string& fs_name) :
        vs_name(vs_name), fs_name(fs_name)
    {
        mLinesPH = LoadProgram(vs_name, fs_name);
        assert(bgfx::isValid(mLinesPH));
    }

    Polylines::Polylines(const Polylines& other) {
        vs_name = other.vs_name;
        fs_name = other.fs_name;

        mSettings = other.mSettings;

        mLinesPH = LoadProgram(vs_name, fs_name);
        assert(bgfx::isValid(mLinesPH));
    }

    Polylines::Polylines(Polylines&& other) {
        vs_name = other.vs_name;
        fs_name = other.fs_name;

        mSettings = other.mSettings;

        mLinesPH = LoadProgram(vs_name, fs_name);
        assert(bgfx::isValid(mLinesPH));
    }

    Polylines::~Polylines() {
        if(bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }
}