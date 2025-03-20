#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class InstancingBasedLines : public GenericLines
    {
        static bgfx::ProgramHandle mPointsPH = LoadProgram("lines/instancing_based_lines/vs_instancing_based_lines", 
                                                           "lines/instancing_based_lines/fs_instancing_based_lines")
        static const inline std::vector<float> mVertices = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        static const inline std::vector<uint32_t> mIndexes = {0, 1, 2, 1, 3, 2};

        std::vector<LinesVertex> mPoints;
        mutable bgfx::InstanceDataBuffer mInstanceDB;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;

    public:
        InstancingBasedLines(const std::vector<LinesVertex> &points);

        InstancingBasedLines(const InstancingBasedLines &other);

        InstancingBasedLines(InstancingBasedLines &&other);

        ~InstancingBasedLines();

        InstancingBasedLines &operator=(InstancingBasedLines other);

        void swap(InstancingBasedLines &other);

        friend void swap(IndirectBasedLines& a, IndirectBasedLines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateInstanceDataBuffer() const;

        void allocateVerticesBuffer();

        void allocateIndexesBuffer();
    };
}