#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class TextureBasedPolylines : public GenericLines<PolylinesSettings>
    {
        static const inline std::vector<float> VERTICES =
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t> INDICES = {0, 3, 1, 0, 2, 3};

        const uint32_t mMaxTextureSize = bgfx::getCaps()->limits.maxTextureSize;

        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "polylines/texture_based_polylines/vs_texture_based_segments",
            "polylines/texture_based_polylines/fs_texture_based_polylines");

        bgfx::ProgramHandle mJointsPH = LoadProgram(
            "polylines/texture_based_polylines/vs_texture_based_joins",
            "polylines/texture_based_polylines/fs_texture_based_polylines");

        bgfx::ProgramHandle mComputeTexturePH = bgfx::createProgram(
            LoadShader("polylines/texture_based_polylines/cs_compute_texture"), true);

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;

        bgfx::TextureHandle mSegmentsTextureBH = BGFX_INVALID_HANDLE;
        bgfx::TextureHandle mJointsTextureBH = BGFX_INVALID_HANDLE;
        
        bgfx::IndirectBufferHandle mSegmentsIndirectBH = bgfx::createIndirectBuffer(1);
        bgfx::IndirectBufferHandle mJointsIndirectBH = bgfx::createIndirectBuffer(1);
        bgfx::UniformHandle mComputeDataUH = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

    public:
        TextureBasedPolylines() { checkCaps(); }

        TextureBasedPolylines(const std::vector<LinesVertex> &points);

        TextureBasedPolylines(const TextureBasedPolylines &other) = delete;

        TextureBasedPolylines(TextureBasedPolylines &&other) = delete;

        ~TextureBasedPolylines();

        TextureBasedPolylines &operator=(TextureBasedPolylines other) = delete;

        void swap(TextureBasedPolylines &other);

        friend void swap(TextureBasedPolylines &a, TextureBasedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps* caps      = bgfx::getCaps();
            const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
            const bool indirectSupported =
                bool(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
            const bool instancingSupported =
                bool(caps->supported & BGFX_CAPS_INSTANCING);
            const bool textureSupported =
                bool(caps->supported & BGFX_CAPS_TEXTURE_2D_ARRAY);

            if (!(instancingSupported && computeSupported && indirectSupported &&
                textureSupported)) {
                throw std::runtime_error(
                    "Instancing or compute or indirect or texture are not "
                    "supported");
            }
        }

        void allocateAndSetPointsBuffer();

        void allocateTextureBuffers();

        void generateTextureBuffers();

        void allocateVerticesAndIndicesBuffers();

    };
}