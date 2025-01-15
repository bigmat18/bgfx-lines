#pragma once
#include "../lines.hpp"

namespace lines {
    class TextureBasedLines : public Lines {

        uint32_t mMaxTextureSize;

        std::vector<float>                  mVertices = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        std::vector<uint32_t>               mIndexes = {0, 1, 2, 1, 3, 2};
        std::vector<LinesVertex>            mPoints;

        bgfx::TextureHandle                 mTextureBH;
        bgfx::DynamicVertexBufferHandle     mPointsBH;
        bgfx::ProgramHandle                 mComputeTexturePH;

        bgfx::IndirectBufferHandle          mIndirectBH;      
        bgfx::UniformHandle                 mIndirectDataUH;

        bgfx::VertexBufferHandle            mVerticesBH;
        bgfx::IndexBufferHandle             mIndexesBH;


        public:
            TextureBasedLines(const std::vector<LinesVertex> &points, const uint32_t maxTextureSize);

            TextureBasedLines(const TextureBasedLines& other);

            TextureBasedLines(TextureBasedLines&& other);

            ~TextureBasedLines();

            TextureBasedLines& operator=(TextureBasedLines other);

            void swap(TextureBasedLines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

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