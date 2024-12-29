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
                Point p1_px = Point((p1.x * m_Data.screenSize[0]) / 2, (p1.y * m_Data.screenSize[1]) / 2, p1.z);
                Point p2_px = Point((p2.x * m_Data.screenSize[0]) / 2, (p2.y * m_Data.screenSize[1]) / 2, p2.z);

                return std::sqrt(std::pow(p1_px.x - p2_px.x, 2) + std::pow(p1_px.y - p2_px.y, 2) + std::pow(p1_px.z - p2_px.z, 2));
            }

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;
    };
}