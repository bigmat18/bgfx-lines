#pragma once
#include <bgfx/bgfx.h>
#include <vector>

#include "lines.hpp"
#include "drawable_lines.hpp"

namespace Lines
{
    class TriangulatedDrawableLines : public DrawableLines
    {
        friend DrawableLines *create(LinesType type, const std::string name);

    public:
        void endLine() override;
        
        void draw(uint viewId) const override;

        void setResolution(float width, float height) {
            mData.resolution[0] = width;
            mData.resolution[1] = height;
        }

        void setColor(float r, float g, float b, float alpha) {
            mData.color[0] = r;
            mData.color[1] = g;
            mData.color[2] = b;
            mData.color[3] = alpha;
        }

        void setAntialis(float antialias) { mData.antialias = antialias; }

        void setThickness(float thickness) { mData.thickness = thickness;}

        std::shared_ptr<vcl::DrawableObjectI> clone() const override { return std::make_shared<TriangulatedDrawableLines>(*this); }

        ~TriangulatedDrawableLines();

    private:
        TriangulatedDrawableLines(const std::string vs_name, 
                                  const std::string fs_name, 
                                  const std::string name = "");

        bgfx::UniformHandle mUniformData;
        bgfx::UniformHandle mUniformColor;
        bgfx::UniformHandle mUniformLength;

        typedef struct
        {
            float resolution[2];
            float antialias;
            float thickness;
            float color[4];
            float length;
        } LinesData;

        LinesData mData;
    };

}