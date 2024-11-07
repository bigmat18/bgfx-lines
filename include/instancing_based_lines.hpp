#pragma once 
#include <lines.hpp>

namespace lines {
    class InstancingBasedLines : public Lines {
        public:
            InstancingBasedLines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~InstancingBasedLines() = default;

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingBasedLines>(*this);
            }

            void draw(uint viewId) const override;

            void setSegments(const std::vector<Segment> &segments);
        
        private:
            bgfx::InstanceDataBuffer m_IDBSegments;

    }; 
}