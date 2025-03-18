#include <lines/primitive_lines.hpp>

namespace lines {
    PrimitiveLines::PrimitiveLines(const std::vector<LinesVertex> &points) :
        Lines("base/vs_base", "base/fs_base"),
        mPoints(points)
    {
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

    PrimitiveLines::PrimitiveLines(const PrimitiveLines& other) :
        Lines(other)
    {
        mPoints = other.mPoints;

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

    PrimitiveLines::PrimitiveLines(PrimitiveLines&& other) :
        Lines(other)
    {
        swap(other);
    }

    PrimitiveLines::~PrimitiveLines() {
        if(bgfx::isValid(mVerticesBH)) 
            bgfx::destroy(mVerticesBH);
    }

    PrimitiveLines& PrimitiveLines::operator=(PrimitiveLines other) {
        swap(other);
        return *this;
    }

    void PrimitiveLines::swap(PrimitiveLines& other) {
        std::swap(mLinesPH, other.mLinesPH);
        std::swap(mSettings, other.mSettings);

        std::swap(mPoints, other.mPoints);
        std::swap(mVerticesBH, other.mVerticesBH);
    }

    void PrimitiveLines::draw(uint32_t viewId) const {
        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_PT_LINES
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setState(state);
        bgfx::submit(viewId, mLinesPH);
    }
}