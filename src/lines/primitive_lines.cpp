#include <lines/primitive_lines.hpp>

namespace lines {
    bgfx::ProgramHandle PrimitiveLines::mLinesPH = LoadProgram("base/vs_base", "base/fs_base");

    PrimitiveLines::PrimitiveLines(const std::vector<LinesVertex> &points) :
        mPoints(points)
    {
        allocateVertexBuffer();
    }

    PrimitiveLines::PrimitiveLines(const PrimitiveLines& other) :
        mPoints(other.mPoints)
    {
        allocateVertexBuffer();
    }

    PrimitiveLines::PrimitiveLines(PrimitiveLines&& other)
    {
        swap(other);
    }

    PrimitiveLines::~PrimitiveLines() {
        if(bgfx::isValid(mVerticesBH)) 
            bgfx::destroy(mVerticesBH);

        if(bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }

    PrimitiveLines& PrimitiveLines::operator=(PrimitiveLines other) {
        swap(other);
        return *this;
    }

    void PrimitiveLines::swap(PrimitiveLines& other) {
        using std::swap;
        GenericLines::swap(other);

        std::swap(mLinesPH, other.mLinesPH);
        std::swap(mPoints, other.mPoints);
        std::swap(mVerticesBH, other.mVerticesBH);
    }

    void PrimitiveLines::draw(uint32_t viewId) const {
        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }

    void PrimitiveLines::allocateVertexBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
         .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()),
            layout
        );
    }
}