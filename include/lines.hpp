#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <bgfx/bgfx.h>
#include <algorithm>

namespace lines {

    struct LinesPoint {
        float x, y, z;

        LinesPoint(float xx, float yy, float zz)
            : x(xx), y(yy), z(zz) {}
    };

    enum LinesType {
        CPU_GENERATED_LINES,
        GPU_GENERATED_LINES,
        INSTANCING_BASED_LINES
    };

    class Lines : public vcl::DrawableObjectI {

        public:

            static std::unique_ptr<Lines> create(const std::vector<LinesPoint> &points, const float width, const float heigth, LinesType type = LinesType::CPU_GENERATED_LINES);

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

            void setColor(float r, float g, float b, float a) { 
                m_Data.color[0] = r;
                m_Data.color[1] = g;
                m_Data.color[2] = b;
                m_Data.color[3] = a;
            }

            void setScreenSize(float width, float heigth) { 
                m_Data.screenSize[0] = width; 
                m_Data.screenSize[1] = heigth;
            }

        protected: 
        
            bgfx::ProgramHandle m_Program;
            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            bgfx::UniformHandle m_UniformData;
            bgfx::UniformHandle m_UniformColor;

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;
            
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
