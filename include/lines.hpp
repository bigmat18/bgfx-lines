#pragma once
#include <bgfx/bgfx.h>
#include <memory>
#include <vector>

#include "lines/cpu_generated_lines.hpp"
#include "lines/gpu_generated_lines.hpp"
#include "lines/instancing_based_lines.hpp"
#include "lines/indirect_based_lines.hpp"
#include "lines/texture_based_lines.hpp"
#include "lines/primitive_based_lines.hpp"

#include "lines_settings.hpp"

namespace lines
{
    template <typename LinesImplementation = CPUGeneratedLines>
    class Lines
    {

        bool                        mVisible = true;
        std::vector<LinesVertex>    mPoints;
        LinesImplementation         mLines;

    public:
        Lines() = default;

        Lines(const std::vector<LinesVertex> &points) : 
            mPoints(points), mLines(points) {}

        Lines(const Lines &other) = delete;

        Lines(Lines &&other) { swap(other); }

        ~Lines() = default;

        Lines &operator=(Lines other)
        {
            swap(other);
            return *this;
        }

        void swap(Lines &other)
        {
            using std::swap;
            swap(mPoints, other.mPoints);
            swap(mLines, other.mLines);
            swap(mVisible, other.mVisible);
        }

        friend void swap(Lines &first, Lines &second)
        {
            first.swap(second);
        }

        const LinesSettings& settings() const { return mLines.settings(); }

        LinesSettings& settings() { return mLines.settings(); }

        void draw(uint32_t viewId) const { mLines.draw(viewId); }

        virtual void update(const std::vector<LinesVertex> &points) 
        {
            mLines.update(points);
        }
    };

    using CPULines         = Lines<CPUGeneratedLines>;
    using GPULines         = Lines<GPUGeneratedLines>;
    using IndirectLines    = Lines<IndirectBasedLines>;
    using InstancingLines  = Lines<InstancingBasedLines>;
    using TextureLines     = Lines<TextureBasedLines>;
    using PrimitiveLines   = Lines<PrimitiveBasedLines>;
}
