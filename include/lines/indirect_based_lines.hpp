#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class IndirectBasedLines : public GenericLines<LinesSettings>
    {
        static inline const std::vector<float> VERTICES = 
            {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        static inline const std::vector<uint32_t> INDICES = 
            {0, 1, 2, 1, 3, 2};

        bgfx::ProgramHandle mComputeIndirectPH = bgfx::createProgram(
            LoadShader("lines/indirect_based_lines/cs_compute_indirect"), true);

        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "lines/indirect_based_lines/vs_indirect_based_lines",
            "lines/indirect_based_lines/fs_indirect_based_lines");

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle mIndirectBH = bgfx::createIndirectBuffer(1);
        bgfx::UniformHandle mIndirectDataUH = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

    public:
        IndirectBasedLines() { checkCaps(); };

        IndirectBasedLines(const std::vector<LinesVertex> &points);

        IndirectBasedLines(const IndirectBasedLines &other) = delete;

        IndirectBasedLines(IndirectBasedLines &&other) = delete;

        IndirectBasedLines &operator=(IndirectBasedLines other) = delete;
        
        ~IndirectBasedLines();

        void swap(IndirectBasedLines &other);

        friend void swap(IndirectBasedLines &a, IndirectBasedLines &b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps *caps = bgfx::getCaps();
            const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
            const bool indirectSupported =
                bool(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
            const bool instancingSupported =
                bool(caps->supported & BGFX_CAPS_INSTANCING);

            if (!(instancingSupported && computeSupported && indirectSupported))
            {
                throw std::runtime_error("Instancing or compute are not supported");
            }
        }

        void allocateVerticesAndIndicesBuffers();

        void allocateAndSetPointsBuffer();

        void generateIndirectBuffer();
    };
}