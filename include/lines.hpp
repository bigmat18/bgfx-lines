#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <bgfx/bgfx.h>
#include <algorithm>

namespace lines {

    struct Point {
        float x, y, z;

        Point(float xx, float yy, float zz)
            : x(xx), y(yy), z(zz) {}
    };

    struct Color {
        float r, g, b, a;

        Color(float rr, float gg, float bb, float aa)
            : r(rr), g(gg), b(bb), a(aa) {}
    };

    struct Segment {
        Point m_P0;
        Point m_P1;
	    Color m_Color;

        Segment(const Point p0, const Point p1, const Color color) :
            m_P0(p0), m_P1(p1), m_Color(color) {}
    };

    enum LinesType {
        CPU_GENERATED_LINES,
        GPU_GENERATED_LINES,
        INSTANCING_BASED_LINES
    };

    class Lines : public vcl::DrawableObjectI {

        public:

            static std::unique_ptr<Lines> create(const std::vector<Segment> &segments, const float width, const float heigth, LinesType type = LinesType::CPU_GENERATED_LINES);

            static std::unique_ptr<Lines> create(bgfx::VertexBufferHandle vbh);

            static std::unique_ptr<Lines> create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh);

            Lines(const float width, const float heigth, const std::string& vs_name,  const std::string& fs_name);

            virtual ~Lines();

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return m_Visible; }

            void setVisibility(bool vis) override { m_Visible = vis; }

            void setThickness(float thickness) { m_Data.thickness = thickness; }

            void setAntialias(float antialias) { m_Data.antialias = antialias; }

            void setScreenSize(float width, float heigth) { 
                m_Data.screenSize[0] = width; 
                m_Data.screenSize[1] = heigth;
            }

        protected: 
        
            bgfx::ProgramHandle m_Program;
            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            bgfx::UniformHandle m_UniformData;

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;
            
            struct LineData {
                float thickness = 5.0;
                float antialias = 0.0;
                float screenSize[2];
            };

            LineData m_Data;
            vcl::Box3d m_BoundingBox;
            bool m_Visible = true;
    };
}
