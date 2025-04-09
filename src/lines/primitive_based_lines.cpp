#include <lines/primitive_based_lines.hpp>

namespace lines
{

    PrimitiveBasedLines::PrimitiveBasedLines(const std::vector<LinesVertex> &points)
    {
        allocateVertexBuffer(points);
    }

    PrimitiveBasedLines::~PrimitiveBasedLines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }

    void PrimitiveBasedLines::swap(PrimitiveBasedLines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        swap(mLinesPH, other.mLinesPH);
        swap(mVerticesBH, other.mVerticesBH);
    }

    void PrimitiveBasedLines::draw(uint32_t viewId) const
    {
        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setState(drawState() | BGFX_STATE_PT_LINES);
        bgfx::submit(viewId, mLinesPH);
    }

    void PrimitiveBasedLines::allocateVertexBuffer(const std::vector<LinesVertex> &points)
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()),
            layout);
    }
}