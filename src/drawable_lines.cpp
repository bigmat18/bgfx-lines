#include <drawable_lines.hpp>
#include <vclib_bgfx/render/context/load_program.h>

namespace Lines {
    DrawableLines::DrawableLines(const std::string vs_name, const std::string fs_name, const std::string name) : 
        mVsName(vs_name), mFsName(fs_name), mClosed(false)
    {
        mProgram = vcl::loadProgram(mVsName, mFsName);
    }

    DrawableLines::~DrawableLines() {
        bgfx::destroy(mIbh);
        bgfx::destroy(mVbh),
        bgfx::destroy(mProgram);
    };

    void DrawableLines::beginLine() {
        mPoints.clear();
        mVertices.clear();
        mIndices.clear();
    };

    void DrawableLines::addPoint(const LinesPoint &point) {
        mPoints.push_back(point);
    }

    float DrawableLines::calculateDistance(const LinesPoint &p1, const LinesPoint &p2) {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }
}
