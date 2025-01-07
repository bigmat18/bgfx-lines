#pragma once 
#include "../lines.hpp"

namespace lines {
    class InstancingBasedLines : public Lines {
        public:
            InstancingBasedLines(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth);

            ~InstancingBasedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingBasedLines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:
            void generateInstanceDataBuffer(const std::vector<Point> &points);

            bgfx::InstanceDataBuffer m_IDBPoints;
            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

    }; 
}