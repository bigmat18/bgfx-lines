#pragma once 
#include <lines.hpp>

namespace lines {
    class InstancingBasedLines : public Lines {
        public:
            InstancingBasedLines(const std::vector<LinesPoint> &points, const float width, const float heigth);

            ~InstancingBasedLines() = default;

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingBasedLines>(*this);
            }

            void draw(uint viewId) const override;

            void setPoints(const std::vector<LinesPoint> &points);
        
        private:
            bgfx::InstanceDataBuffer m_IDBPoints;

    }; 
}