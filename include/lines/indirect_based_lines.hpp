#pragma once
#include "../lines.hpp"

namespace lines {
class IndirectBasedLines : public Lines {
        public:
            IndirectBasedLines(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth);

            ~IndirectBasedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<IndirectBasedLines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:

            void allocatePointsBuffer();

            void generateIndirectBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;
            bgfx::DynamicVertexBufferHandle m_PointsBuffer;

            bgfx::IndirectBufferHandle m_IndirectBuffer;
            bgfx::ProgramHandle m_ComputeIndirect;            
            bgfx::UniformHandle m_IndirectDataUniform;

            uint32_t m_PointsSize;
    };  
}