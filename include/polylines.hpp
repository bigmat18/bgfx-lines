#pragma once
#include <bgfx/bgfx.h>
#include <memory>
#include <vector>
#include "lines_settings.hpp"
#include "utils.hpp"

namespace lines {
    class Polylines {

        public:
            static std::unique_ptr<Polylines> create(const std::vector<LinesVertex> &points, LinesTypes type = LinesTypes::CPU_GENERATED);

            Polylines(const std::string& vs_name, const std::string& fs_name);

            Polylines(const Polylines& other);

            Polylines(Polylines&& other);

            virtual ~Polylines();

            LinesSettings& getSettings() { return mSettings; }

            virtual void draw(uint32_t viewId) const = 0;

            virtual void update(const std::vector<LinesVertex> &points) = 0;

        protected: 
            std::string vs_name;
            std::string fs_name;
            
            bgfx::ProgramHandle         mLinesPH;
            LinesSettings               mSettings;
    };
}