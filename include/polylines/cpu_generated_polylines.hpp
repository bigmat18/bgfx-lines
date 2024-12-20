#pragma once
#include "../polylines.hpp"

namespace lines {
    class CPUGeneratedPolylines : public Polylines {

        public:
            CPUGeneratedPolylines(const std::vector<Point> &points, const float width, const float heigth);

            ~CPUGeneratedPolylines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:
            void generateVertexBuffer(const std::vector<Point> points);

            void generateIndexBuffer(const std::vector<Point> points);

            inline float calculateDistance(const Point &p1, const Point &p2) {
                return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
            }

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;
    };
}