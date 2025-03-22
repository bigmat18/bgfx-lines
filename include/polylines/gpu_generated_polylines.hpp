#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class GPUGeneratedPolylines : public GenericLines<PolylinesSettings>
    {
        static const bgfx::ProgramHandle mComputeVertexPH;
        static const bgfx::ProgramHandle mLinesPH;

        std::vector<LinesVertex> mPoints;

        bgfx::DynamicVertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle mVertexBH = BGFX_INVALID_HANDLE;

        bgfx::DynamicIndexBufferHandle mSegmentsIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle mJointIndicesBH = BGFX_INVALID_HANDLE;

        bgfx::UniformHandle mComputeDataUH = BGFX_INVALID_HANDLE;

    public:
        GPUGeneratedPolylines(const std::vector<LinesVertex> &points);

        GPUGeneratedPolylines(const GPUGeneratedPolylines &other);

        GPUGeneratedPolylines(GPUGeneratedPolylines &&other);

        ~GPUGeneratedPolylines();

        GPUGeneratedPolylines &operator=(GPUGeneratedPolylines other);

        void swap(GPUGeneratedPolylines &other);

        friend void swap(GPUGeneratedPolylines& a, GPUGeneratedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateBuffers();

        void allocateVertexBuffer();

        void allocateIndexBuffer();

        void allocatePointsBuffer();
    };
}