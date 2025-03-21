#pragma once
#include "../generic_lines.hpp"

namespace lines
{

    class PrimitiveLines : public GenericLines<LinesSettings>
    {

        static bgfx::ProgramHandle mLinesPH;
        std::vector<LinesVertex> mPoints;
        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;

    public:
        PrimitiveLines(const std::vector<LinesVertex> &points);

        PrimitiveLines(const PrimitiveLines &other);

        PrimitiveLines(PrimitiveLines &&other);

        ~PrimitiveLines();

        PrimitiveLines &operator=(PrimitiveLines other);

        void swap(PrimitiveLines &other);

        friend void swap(PrimitiveLines &a, PrimitiveLines &b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override {};

    private:
        inline void allocateVertexBuffer();
    };
}