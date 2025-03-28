#pragma once
#include "../generic_lines.hpp"

namespace lines
{

    class CPUGeneratedLines : public GenericLines<LinesSettings>
    {

        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "lines/cpu_generated_lines/vs_cpu_generated_lines", 
            "lines/cpu_generated_lines/fs_cpu_generated_lines");

        std::vector<lines::LinesVertex> mPoints;
        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;

    public:
        CPUGeneratedLines() = default;

        CPUGeneratedLines(const std::vector<LinesVertex> &points);

        CPUGeneratedLines(const CPUGeneratedLines &other) = delete;

        CPUGeneratedLines(CPUGeneratedLines &&other);

        ~CPUGeneratedLines();

        CPUGeneratedLines &operator=(CPUGeneratedLines other);

        void swap(CPUGeneratedLines &other);

        friend void swap(CPUGeneratedLines& a, CPUGeneratedLines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateVerticesAndIndicesBuffers(const std::vector<LinesVertex> &points);
    };
}