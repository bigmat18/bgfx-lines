#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <bgfx/bgfx.h>
#include <algorithm>
#include "lines_settings.hpp"
#include "utils.hpp"

namespace lines {

    class Lines : public vcl::DrawableObjectI {

        public:

            static std::unique_ptr<Lines> create(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth, Types type = Types::CPU_GENERATED);

            static std::unique_ptr<Lines> create(bgfx::VertexBufferHandle vbh);

            static std::unique_ptr<Lines> create(bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ivh);

            Lines(const uint16_t width, const uint16_t heigth, const std::string& vs_name, const std::string& fs_name);

            virtual ~Lines();

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return m_Visible; }

            void setVisibility(bool vis) override { m_Visible = vis; }

            LinesSettings& getSettings() { return m_Settings; }

            virtual void update(const std::vector<Point> &points) = 0;

        protected: 
            bgfx::ProgramHandle m_Program;
            vcl::Box3d m_BoundingBox;

            LinesSettings m_Settings;
            bool m_Visible = true;
    };
}
