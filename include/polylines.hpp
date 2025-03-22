#pragma once
#include <bgfx/bgfx.h>
#include <memory>
#include <vector>
#include "lines_settings.hpp"
#include "lines_utils.hpp"

#include "polylines/cpu_generated_polylines.hpp"
#include "polylines/gpu_generated_polylines.hpp"
#include "polylines/instancing_based_polylines.hpp"
#include "polylines/indirect_based_polylines.hpp"
#include "polylines/texture_based_polylines.hpp"

namespace lines {
    template <typename PolylinesImplementation = CPUGeneratedPolylines>
    class Polylines
    {

        bool                        mVisible = true;
        std::vector<LinesVertex>    mPoints;
        PolylinesImplementation     mLines;

    public:
        Polylines() = default;

        Polylines(const std::vector<LinesVertex> &points) : mPoints(points), mLines(points) {}

        Polylines(const Polylines &other) : mPoints(other.mPoints),
                                    mLines(other.mPoints),
                                    mVisible(other.mVisible)
        {
            mLines.settings() = other.settings();
        }

        Polylines(Polylines &&other) { swap(other); }

        ~Polylines() = default;

        Polylines &operator=(Polylines other)
        {
            swap(other);
            return *this;
        }

        void swap(Polylines &other)
        {
            using std::swap;
            swap(mPoints, other.mPoints);
            swap(mLines, other.mLines);
            swap(mVisible, other.mVisible);
        }

        friend void swap(Polylines &first, Polylines &second)
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

    using CPUPolylines         = Polylines<CPUGeneratedPolylines>;
    using GPUPolylines         = Polylines<GPUGeneratedPolylines>;
    using IndirectPolylines    = Polylines<IndirectBasedPolylines>;
    using InstancingPolylines  = Polylines<InstancingBasedPolylines>;
    using TexturePolylines     = Polylines<TextureBasedPolylines>;
}