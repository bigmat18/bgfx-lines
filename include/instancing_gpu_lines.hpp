#pragma once
#include "lines.hpp"

namespace lines {
class InstancingGPULines : public Lines {
        public:
            InstancingGPULines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~InstancingGPULines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingGPULines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Segment> &segments) override;

        private:

            void allocateTextureBuffer();

            void allocateSegmentsBuffer();

            void generateIndirectBuffer();

            void generateTextureBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;
            
            bgfx::TextureHandle m_TextureBuffer;
            bgfx::UniformHandle m_TextureUniform;
            
            bgfx::DynamicVertexBufferHandle m_SegmentsBuffer;
            
            bgfx::ProgramHandle m_ComputeProgram;
            bgfx::ProgramHandle m_ComputeIndirect;
            
            bgfx::IndirectBufferHandle m_IndirectBuffer;
            bgfx::UniformHandle m_IndirectDataUniform;

            uint32_t m_SegmentsSize;
    };  
}