#pragma once 
#include <lines.hpp>

namespace lines {

    class CPUGeneratedLines : public Lines {

        public:
            CPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~CPUGeneratedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedLines>(*this);
            }

            void draw(uint viewId) const override;

        private:
            void generateVertexBuffer(const std::vector<Segment> segments);

            void generateIndexBuffer(const std::vector<Segment> segments);

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

    };
}