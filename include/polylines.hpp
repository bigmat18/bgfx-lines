#pragma once
#include <vclib/render/interfaces/drawable_object_i.h>
#include <bgfx/bgfx.h>
#include "lines_settings.hpp"
#include "utils.hpp"

namespace lines {
    class Polylines : public vcl::DrawableObjectI {

        public:
            static std::unique_ptr<Polylines> create(const std::vector<LinesVertex> &points, LinesTypes type = LinesTypes::CPU_GENERATED);

            Polylines(const std::string& vs_name, const std::string& fs_name);

            virtual ~Polylines();

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return m_Visible; }

            void setVisibility(bool vis) override { m_Visible = vis; }

            LinesSettings& getSettings() { return m_Settings; }

            virtual void update(const std::vector<LinesVertex> &points) = 0;

        protected: 
            bgfx::ProgramHandle m_Program;
            
            LinesSettings m_Settings;
            vcl::Box3d m_BoundingBox;
            bool m_Visible = true;
    };
}