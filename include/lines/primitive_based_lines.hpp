#pragma once
#include "../generic_lines.hpp"

namespace lines
{

    class PrimitiveBasedLines : public GenericLines<LinesSettings>
    {

        bgfx::ProgramHandle mLinesPH = LoadProgram("base/vs_base", "base/fs_base");
        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;

    public:
        PrimitiveBasedLines() = default;

        PrimitiveBasedLines(const std::vector<LinesVertex> &points);

        PrimitiveBasedLines(const PrimitiveBasedLines &other) = delete;

        PrimitiveBasedLines(PrimitiveBasedLines &&other) = delete;

        PrimitiveBasedLines &operator=(PrimitiveBasedLines other) = delete;
        
        ~PrimitiveBasedLines();

        void swap(PrimitiveBasedLines &other);

        friend void swap(PrimitiveBasedLines &a, PrimitiveBasedLines &b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override {};

    private:
        void allocateVertexBuffer(const std::vector<lines::LinesVertex> &points);
    };
}