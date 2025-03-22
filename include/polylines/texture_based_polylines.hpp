#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class TextureBasedPolylines : public GenericLines<PolylinesSettings>
    {
        static const bgfx::ProgramHandle mLinesPH;
        static const bgfx::ProgramHandle mJointsPH;
        static const bgfx::ProgramHandle mComputeTexturePH;

        const uint32_t mMaxTextureSize = bgfx::getCaps()->limits.maxTextureSize;
        static const inline std::vector<float> mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t> mIndices = {0, 3, 1, 0, 2, 3};

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle mSegmentsIndirectBH = BGFX_INVALID_HANDLE;
        bgfx::IndirectBufferHandle mJointsIndirectBH = BGFX_INVALID_HANDLE;

        bgfx::TextureHandle mSegmentsTextureBH = BGFX_INVALID_HANDLE;
        bgfx::TextureHandle mJointsTextureBH = BGFX_INVALID_HANDLE;

        bgfx::UniformHandle mComputeDataUH = BGFX_INVALID_HANDLE;

    public:
        TextureBasedPolylines(const std::vector<LinesVertex> &points);

        TextureBasedPolylines(const TextureBasedPolylines &other);

        TextureBasedPolylines(TextureBasedPolylines &&other);

        ~TextureBasedPolylines();

        TextureBasedPolylines &operator=(TextureBasedPolylines other);

        void swap(TextureBasedPolylines &other);

        friend void swap(TextureBasedPolylines &a, TextureBasedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateTextureBuffer();

        void allocateTextureBuffer();

        void allocatePointsBuffer();

        void allocateVerticesBuffer();

        void allocateIndexesBuffer();
    };
}