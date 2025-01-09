#pragma once
#include "../polylines.hpp"

namespace lines {
    class CPUGeneratedPolylines : public Polylines {

        public:
            CPUGeneratedPolylines(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth);

            ~CPUGeneratedPolylines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:
            void generateBuffers(const std::vector<Point> points);

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_SegmentsIndices;
            std::vector<uint32_t> m_JoinsIndices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_SegmentsIbh;
            bgfx::IndexBufferHandle m_JoinsIbh;

    };
}