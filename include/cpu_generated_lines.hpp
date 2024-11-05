#pragma once 
#include <lines.hpp>

namespace lines {

    class CPUGeneratedLines : public Lines {

        public:
            CPUGeneratedLines(const std::vector<LinesPoint> &points, const float width, const float heigth);

            ~CPUGeneratedLines() = default;

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedLines>(*this);
            }

            void draw(uint viewId) const override;

        private:
            void generateVertexBuffer(const std::vector<LinesPoint> points);

            void generateIndexBuffer(const std::vector<LinesPoint> points);

    };
}