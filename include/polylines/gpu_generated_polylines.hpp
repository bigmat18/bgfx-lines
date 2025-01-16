#pragma once
#include "../polylines.hpp"

namespace lines {
    class GPUGeneratedPolylines : public Polylines {

        std::vector<LinesVertex>            mPoints;

        bgfx::DynamicVertexBufferHandle     mPointsBH;
        bgfx::DynamicVertexBufferHandle     mVertexBH;
        bgfx::DynamicIndexBufferHandle      mSegmentsIndexesBH;
        bgfx::DynamicIndexBufferHandle      mJoinesIndexesBH;
            
        bgfx::ProgramHandle                 mComputeVertexPH;
        bgfx::UniformHandle                 mComputeDataUH;

        public:
            GPUGeneratedPolylines(const std::vector<LinesVertex> &points);

            GPUGeneratedPolylines(const GPUGeneratedPolylines& other);

            GPUGeneratedPolylines(GPUGeneratedPolylines&& other);

            ~GPUGeneratedPolylines();

            GPUGeneratedPolylines& operator=(GPUGeneratedPolylines other);

            void swap(GPUGeneratedPolylines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();
    };
}