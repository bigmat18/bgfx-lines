#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class CPUGeneratedPolylines : public GenericLines<PolylinesSettings>
    {
        static const bgfx::ProgramHandle mLinesPH;
        std::vector<LinesVertex> mPoints;

        bgfx::DynamicVertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle mSegmentsIndexesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle mJoinsIndexesBH = BGFX_INVALID_HANDLE;

    public:
        CPUGeneratedPolylines(const std::vector<LinesVertex> &points);

        CPUGeneratedPolylines(const CPUGeneratedPolylines &other);

        CPUGeneratedPolylines(CPUGeneratedPolylines &&other);

        ~CPUGeneratedPolylines();

        CPUGeneratedPolylines &operator=(CPUGeneratedPolylines other);

        void swap(CPUGeneratedPolylines &other);

        friend void swap(CPUGeneratedPolylines& a, CPUGeneratedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateBuffers();

        void allocateVertexBuffer();

        void allocateIndexesBuffer();
    };
}