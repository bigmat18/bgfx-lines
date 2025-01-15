#pragma once 
#include "../lines.hpp"

namespace lines {

    class CPUGeneratedLines : public Lines {

        public:
            CPUGeneratedLines(const std::vector<LinesVertex> &points);

            ~CPUGeneratedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<CPUGeneratedLines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers(const std::vector<LinesVertex> points);

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::DynamicVertexBufferHandle m_Vbh;
            bgfx::DynamicIndexBufferHandle m_Ibh;
            uint32_t m_PointsSize;

    };
}