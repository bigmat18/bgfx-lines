#pragma once
#include <vclib/render/drawable/drawable_object.h>
#include <bgfx/bgfx.h>
#include "lines_settings.hpp"
#include "utils.hpp"

namespace lines {
    class Polylines : public vcl::DrawableObject {

        public:
            static std::unique_ptr<Polylines> create(const std::vector<LinesVertex> &points, LinesTypes type = LinesTypes::CPU_GENERATED);

            Polylines(const std::string& vs_name, const std::string& fs_name);

            Polylines(const Polylines& other);

            Polylines(Polylines&& other);

            virtual ~Polylines();

            vcl::Box3d boundingBox() const override { 
                return vcl::Box3d(vcl::Point3d(-1,-1,-1), vcl::Point3d(1, 1, 1));
            }

            bool isVisible() const override { return mVisible; }

            void setVisibility(bool vis) override { mVisible = vis; }

            LinesSettings& getSettings() { return mSettings; }

            virtual void update(const std::vector<LinesVertex> &points) = 0;

        protected: 
            bool mVisible = true;
            std::string vs_name;
            std::string fs_name;
            
            bgfx::ProgramHandle         mLinesPH;
            LinesSettings               mSettings;
    };
}