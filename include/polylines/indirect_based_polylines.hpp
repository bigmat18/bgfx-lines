#pragma once
#include "../polylines.hpp"

namespace lines {
    class IndirectBasedPolylines : public Polylines {

        static const inline std::vector<float>       mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t>    mIndexes = {0, 3, 1, 0, 2, 3};
        
        std::vector<LinesVertex>                    mPoints;

        bgfx::VertexBufferHandle                    mVerticesBH             = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle                     mIndexesBH              = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle             mPointsBH               = BGFX_INVALID_HANDLE;
        bgfx::IndirectBufferHandle                  mSegmentsIndirectBH     = BGFX_INVALID_HANDLE;
        bgfx::IndirectBufferHandle                  mJoinesIndirectBH       = BGFX_INVALID_HANDLE;

        bgfx::ProgramHandle                         mJoinesPH               = BGFX_INVALID_HANDLE;
        bgfx::ProgramHandle                         mComputeIndirectPH      = BGFX_INVALID_HANDLE;            
        bgfx::UniformHandle                         mComputeIndirectDataUH  = BGFX_INVALID_HANDLE;

        public:
            IndirectBasedPolylines(const std::vector<LinesVertex> &points);

            IndirectBasedPolylines(const IndirectBasedPolylines& other);

            IndirectBasedPolylines(IndirectBasedPolylines&& other);

            ~IndirectBasedPolylines();

            IndirectBasedPolylines& operator=(IndirectBasedPolylines other);

            void swap(IndirectBasedPolylines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateIndirectBuffers();

            void allocatePointsBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffers();
    };
}
