#pragma once
#include "../polylines.hpp"

namespace lines {
    class TextureBasedPolylines : public Polylines {

        uint32_t mMaxTextureSize;
        
        std::vector<LinesVertex>            mPoints;
        std::vector<float>                  mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        std::vector<uint32_t>               mIndexes = {0, 3, 1, 0, 2, 3};

        bgfx::VertexBufferHandle            mVerticesBH             = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle             mIndexesBH              = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mPointsBH               = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle          mSegmentsIndirectBH     = BGFX_INVALID_HANDLE;
        bgfx::IndirectBufferHandle          mJoinesIndirectBH       = BGFX_INVALID_HANDLE;

        bgfx::TextureHandle                 mSegmentsTextureBH      = BGFX_INVALID_HANDLE;
        bgfx::TextureHandle                 mJoinesTextureBH        = BGFX_INVALID_HANDLE;
            
        bgfx::ProgramHandle                 mJoinesPH               = BGFX_INVALID_HANDLE;      
        bgfx::ProgramHandle                 mComputeTexturePH       = BGFX_INVALID_HANDLE; 
        bgfx::UniformHandle                 mComputeDataUH          = BGFX_INVALID_HANDLE;        

        public:
            TextureBasedPolylines(const std::vector<LinesVertex> &points, const uint32_t maxTextureSize);

            TextureBasedPolylines(const TextureBasedPolylines& other);

            TextureBasedPolylines(TextureBasedPolylines&& other);

            ~TextureBasedPolylines();

            TextureBasedPolylines& operator=(TextureBasedPolylines other);

            void swap(TextureBasedPolylines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateTextureBuffer();

            void allocateTextureBuffer();

            void allocatePointsBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    };
}