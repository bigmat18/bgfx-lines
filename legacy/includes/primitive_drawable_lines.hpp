#pragma once
#include "lines.hpp"
#include "drawable_lines.hpp"

namespace Lines
{

    class PrimitiveDrawableLines : public DrawableLines
    {
        friend DrawableLines *create(LinesType type, const std::string name);

    public:
        void endLine() override;
        
        void draw(uint viewId) const override;

        void setColor(float r, float g, float b, float alpha)
        {
            mColor[0] = r;
            mColor[1] = g;
            mColor[2] = b;
            mColor[3] = alpha;
        }

        ~PrimitiveDrawableLines() = default;

        std::shared_ptr<vcl::DrawableObject> clone() const override { 
            return std::make_shared<PrimitiveDrawableLines>(*this); 
        }

    private:
        PrimitiveDrawableLines(const std::string vs_name, 
                               const std::string fs_name, 
                               const std::string name = "");

        float mColor[4];
        bgfx::UniformHandle mUniformColor;
    };
}