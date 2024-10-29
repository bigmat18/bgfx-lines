#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>

namespace lines {

        
    struct LinePoint
    {
        float x, y, z;

        LinesPoint(float xx, float yy, float zz) 
            : x(xx), y(yy), z(zz) {}
    };

    class TriangulatedLines : public vcl::DrawableObjectI {
        
        public:
            TriangulatedLines(std::vector<LinePoint> points);

            TriangulatedLines(bgfx::ProgramHandle program, bgfx::VertexBufferHandle vbh);

            TriangulatedLines(bgfx::ProgramHandle program, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh);

            vcl::Box3d boundingBox() const override { return m_BoundingBox}

            bool isVisible() const override { return m_Visible; }

            void setVisibility(bool vis) override { m_Visible = vis; }

            std::shared_ptr<vcl::DrawableObjectI> clone() const override { 
                return std::make_shared<TriangulatedLines>(*this); 
            }

            void draw(uint viewId) const override;

        private:
            vcl::Box3d m_BoundingBox;
            bool m_Visible;
            bgfx::ProgramHandle m_Program;
            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;
    };
}
