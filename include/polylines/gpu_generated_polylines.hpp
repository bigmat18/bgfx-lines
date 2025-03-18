#pragma once
#include "../polylines.hpp"

namespace lines {
    class GPUGeneratedPolylines : public Polylines {

        std::vector<LinesVertex>            mPoints;

        bgfx::DynamicVertexBufferHandle     mPointsBH               = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mVertexBH               = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle      mSegmentsIndexesBH      = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle      mJoinesIndexesBH        = BGFX_INVALID_HANDLE;
            
        bgfx::ProgramHandle                 mComputeVertexPH        = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle                 mComputeDataUH          = BGFX_INVALID_HANDLE;

        public:
            GPUGeneratedPolylines(const std::vector<LinesVertex> &points);

            GPUGeneratedPolylines(const GPUGeneratedPolylines& other);

            GPUGeneratedPolylines(GPUGeneratedPolylines&& other);

            ~GPUGeneratedPolylines();

            GPUGeneratedPolylines& operator=(GPUGeneratedPolylines other);

            void swap(GPUGeneratedPolylines& other);

            void draw(uint32_t viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();
    };
}