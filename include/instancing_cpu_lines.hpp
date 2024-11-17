#pragma once 
#include <lines.hpp>

namespace lines {
    class InstancingCPULines : public Lines {
        public:
            InstancingCPULines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~InstancingCPULines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingCPULines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Segment> &segments) override;

        private:
            void generateInstanceDataBuffer(const std::vector<Segment> &segments);

            bgfx::InstanceDataBuffer m_IDBSegments;
            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

    }; 
}