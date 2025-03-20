#pragma once
#include "../generic_lines.hpp"

namespace lines
{

    class CPUGeneratedLines : public GenericLines<LinesSettings>
    {

        static bgfx::ProgramHandle mLinesPH = LoadProgram("lines/cpu_generated_lines/vs_cpu_generated_lines",
                                                          "lines/cpu_generated_lines/fs_cpu_generated_lines");
        uint32_t mPointsSize;

        bgfx::DynamicVertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;

    public:
        CPUGeneratedLines() = default;

        CPUGeneratedLines(const std::vector<LinesVertex> &points);

        CPUGeneratedLines(const CPUGeneratedLines &other);

        CPUGeneratedLines(CPUGeneratedLines &&other);

        ~CPUGeneratedLines();

        CPUGeneratedLines &operator=(CPUGeneratedLines other);

        void swap(CPUGeneratedLines &other);

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateBuffers(const std::vector<LinesVertex> &points);

        void allocateVertexBuffer();

        void allocateIndexBuffer();
    };
}