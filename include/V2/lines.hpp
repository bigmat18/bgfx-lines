#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <bgfx/bgfx.h>
#include <algorithm>

namespace lines
{

    struct LinePoint {
        float x, y, z;

        LinePoint(float xx, float yy, float zz)
            : x(xx), y(yy), z(zz) {}
    };

    class TriangulatedLines : public vcl::DrawableObjectI {

    public:
        TriangulatedLines(const std::vector<LinePoint> &points, const float width, const float heigth);

        TriangulatedLines(const bgfx::ProgramHandle program, const bgfx::VertexBufferHandle vbh);

        TriangulatedLines(const bgfx::ProgramHandle program, const bgfx::VertexBufferHandle vbh, const bgfx::IndexBufferHandle ibh);

        ~TriangulatedLines();

        vcl::Box3d boundingBox() const override { 
            return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
         }

        bool isVisible() const override { return true; }

        void setVisibility(bool vis) override { m_Visible = vis; }

        std::shared_ptr<vcl::DrawableObjectI> clone() const override {
            return std::make_shared<TriangulatedLines>(*this);
        }

        void draw(uint viewId) const override;

        void setColor(float color[4]) { std::copy(color, color + 4, std::begin(m_Data.color)); }

        void setThickness(float thickness) { m_Data.thickness = thickness; }

        void setAntialias(float antialias) { m_Data.antialias = antialias; }

        void setScreenSize(float width, float heigth) {
            m_Data.screenSize[0] = width;
            m_Data.screenSize[1] = heigth;
        }

    private:
        void generateVertexBuffer(const std::vector<LinePoint> points);

        void generateIndexBuffer(const std::vector<LinePoint> points);

    private: 
    
        bgfx::ProgramHandle m_Program;
        bgfx::VertexBufferHandle m_Vbh;
        bgfx::IndexBufferHandle m_Ibh;

        bgfx::UniformHandle m_UniformData;
        bgfx::UniformHandle m_UniformColor;

        struct LineData {
            float thickness = 5.0;
            float antialias = 0.0;
            float color[4] = {0.0, 1.0, 0.0, 1.0};
            float screenSize[2];
        };

        LineData m_Data;
        vcl::Box3d m_BoundingBox;
        bool m_Visible = true;
    };
}
