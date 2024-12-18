#pragma once
#include "../lines.hpp"

namespace lines {
    class TextureBasedLines : public Lines {
        public:
            TextureBasedLines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~TextureBasedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<TextureBasedLines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Segment> &segments) override;

        private:
            void generateIndirectBuffer();

            void generateTextureBuffer();

            void allocateTextureBuffer();

            void allocateSegmentsBuffer();

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::TextureHandle m_TextureBuffer;
            bgfx::DynamicVertexBufferHandle m_SegmentsBuffer;
            bgfx::ProgramHandle m_ComputeTexture;            


            bgfx::IndirectBufferHandle m_IndirectBuffer;
            bgfx::ProgramHandle m_ComputeIndirect;            
            bgfx::UniformHandle m_IndirectDataUniform;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            uint32_t m_SegmentsSize;
    };
}