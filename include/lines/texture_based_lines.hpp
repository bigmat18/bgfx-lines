#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class TextureBasedLines : public GenericLines<LinesSettings>
    {
        static inline const std::vector<float> VERTICES =
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        static inline const std::vector<uint32_t> INDICES = 
            {0, 1, 2, 1, 3, 2};

        bgfx::ProgramHandle mComputeTexturePH = bgfx::createProgram(
            LoadShader("lines/texture_based_lines/cs_compute_texture"), true);
            
        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "lines/texture_based_lines/vs_texture_based_lines",
            "lines/texture_based_lines/fs_texture_based_lines");
            
        bgfx::IndirectBufferHandle mIndirectBH = bgfx::createIndirectBuffer(1);
        bgfx::UniformHandle mIndirectDataUH = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

        const uint32_t mMaxTextureSize = bgfx::getCaps()->limits.maxTextureSize;
        std::vector<LinesVertex> mPoints;

        bgfx::TextureHandle mTextureBH = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;


        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;

    public:
        TextureBasedLines() { checkCaps(); }

        TextureBasedLines(const std::vector<LinesVertex> &points);

        TextureBasedLines(const TextureBasedLines &other) = delete;

        TextureBasedLines(TextureBasedLines &&other) = delete;

        TextureBasedLines &operator=(TextureBasedLines other) = delete;
        
        ~TextureBasedLines();

        void swap(TextureBasedLines &other);

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps* caps = bgfx::getCaps();
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

        void allocateAndGenerateTextureBuffer();

        void allocateVerticesAndIndicesBuffers();

    };
}