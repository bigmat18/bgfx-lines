#pragma once
#include "../lines.hpp"

namespace lines {
    class TextureBasedLines : public Lines {

        static const inline std::vector<float>     mVertices = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        static const inline std::vector<uint32_t>  mIndexes = {0, 1, 2, 1, 3, 2};
        
        uint32_t                                    mMaxTextureSize;
        std::vector<LinesVertex>                    mPoints;

        bgfx::TextureHandle                         mTextureBH              = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle             mPointsBH               = BGFX_INVALID_HANDLE;
        bgfx::ProgramHandle                         mComputeTexturePH       = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle                  mIndirectBH             = BGFX_INVALID_HANDLE;      
        bgfx::UniformHandle                         mIndirectDataUH         = BGFX_INVALID_HANDLE;

        bgfx::VertexBufferHandle                    mVerticesBH             = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle                     mIndexesBH              = BGFX_INVALID_HANDLE;


        public:
            TextureBasedLines(const std::vector<LinesVertex> &points, const uint32_t maxTextureSize);

            TextureBasedLines(const TextureBasedLines& other);

            TextureBasedLines(TextureBasedLines&& other);

            ~TextureBasedLines();

            TextureBasedLines& operator=(TextureBasedLines other);

            void swap(TextureBasedLines& other);

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