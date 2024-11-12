#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <bgfx/bgfx.h>
#include "utils.hpp"

namespace lines {
    class Polylines : public vcl::DrawableObjectI {

        public:
            static std::unique_ptr<Polylines> create(const std::vector<Point> &points, const float width, const float heigth, Types type = Types::CPU_GENERATED);

            static std::unique_ptr<Polylines> create(bgfx::VertexBufferHandle vbh);

            static std::unique_ptr<Polylines> create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh);


            Polylines(const float width, const float heigth, const std::string& vs_name,  const std::string& fs_name);

            virtual ~Polylines();


            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return m_Visible; }

            void setVisibility(bool vis) override { m_Visible = vis; }

            virtual void update(const std::vector<Point> &points) = 0;

            void setThickness(float thickness) { m_Data.thickness = thickness; }

            void setAntialias(float antialias) { m_Data.antialias = antialias; }

            void setColor(Color color) { m_Data.color = color; }

            void setScreenSize(float width, float heigth) { 
                m_Data.screenSize[0] = width; 
                m_Data.screenSize[1] = heigth;
            }

        protected: 
            bgfx::ProgramHandle m_Program;
            bgfx::UniformHandle m_UniformData;
            bgfx::UniformHandle m_UniformColor;
            
            struct LineData {
                float thickness = 5.0;
                float antialias = 0.0;
                float screenSize[2];
                Color color;
            };

            LineData m_Data;
            vcl::Box3d m_BoundingBox;
            bool m_Visible = true;
    };
}