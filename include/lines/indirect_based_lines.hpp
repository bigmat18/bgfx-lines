#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class IndirectBasedLines : public GenericLines<LinesSettings>
    {

        static bgfx::ProgramHandle mComputeIndirectPH;
        static bgfx::ProgramHandle mLinesPH;

        static const inline std::vector<float> mVertices = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        static const inline std::vector<uint32_t> mIndexes = {0, 1, 2, 1, 3, 2};

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle mIndirectBH = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle mIndirectDataUH = BGFX_INVALID_HANDLE;

    public:
        IndirectBasedLines(const std::vector<LinesVertex> &points);

        IndirectBasedLines(const IndirectBasedLines &other);

        IndirectBasedLines(IndirectBasedLines &&other);

        ~IndirectBasedLines();

        IndirectBasedLines &operator=(IndirectBasedLines other);

        void swap(IndirectBasedLines &other);

        friend void swap(IndirectBasedLines &a, IndirectBasedLines &b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void allocatePointsBuffer();

        void allocateVerticesBuffer();

        void allocateIndexesBuffer();

        void generateIndirectBuffer();
    };
}