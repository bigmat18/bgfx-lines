#pragma once
#include "../generic_lines.hpp"

namespace lines
{

    class CPUGeneratedLines : public GenericLines<LinesSettings>
    {

        bgfx::ProgramHandle mLinesPH;
        std::vector<lines::LinesVertex> mPoints;

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

        friend void swap(CPUGeneratedLines& a, CPUGeneratedLines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateBuffers();

        void allocateVertexBuffer();

        void allocateIndexBuffer();
    };
}