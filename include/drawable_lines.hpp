#pragma once
#include <bgfx/bgfx.h>
#include <vector>
#include <string>

#include <vclib/render/interfaces/drawable_object_i.h>
#include "lines.hpp"

namespace Lines
{

    class DrawableLines : public vcl::DrawableObjectI
    {
        friend DrawableLines *create(LinesType type, const std::string name);

    public:
        void beginLine();
        void addPoint(const LinesPoint &point);
        virtual void endLine() = 0;

        void setClosed(bool closed) { mClosed = closed; }
        bool isClosed() const { return mClosed; }

        virtual ~DrawableLines();

        vcl::Box3d boundingBox() const override { return vcl::Box3d(); }

        bool isVisible() const override { return true; }

        void setVisibility(bool vis) override {}


    protected:
        DrawableLines(const std::string vs_name,
                      const std::string fs_name,
                      const std::string name);

        float calculateDistance(const LinesPoint &p1, const LinesPoint &p2);

        bgfx::ProgramHandle mProgram;
        bgfx::VertexBufferHandle mVbh;
        bgfx::IndexBufferHandle mIbh;
        bgfx::VertexLayout mLayout;

        std::vector<float> mVertices;
        std::vector<uint32_t> mIndices;

        const std::string mVsName;
        const std::string mFsName;

        std::vector<LinesPoint> mPoints;
        bool mClosed;
    };
}