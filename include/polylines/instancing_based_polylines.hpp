#pragma once
#include "../polylines.hpp"

namespace lines {
    class InstancingBasedPolylines : public Polylines {
        public:
            InstancingBasedPolylines(const std::vector<Point> &points, const float width, const float heigth);

            ~InstancingBasedPolylines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingBasedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:
            void generateInstanceDataBuffer(const std::vector<Point> &points);

            bgfx::InstanceDataBuffer m_InstanceDataBuffer;
            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

    };
}