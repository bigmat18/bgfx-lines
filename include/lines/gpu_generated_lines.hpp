#pragma once
#include "../lines.hpp"

namespace lines {
    class GPUGeneratedLines : public Lines {

        public:
            GPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~GPUGeneratedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<GPUGeneratedLines>(*this);
            };

            void draw(uint viewId) const override;

            void update(const std::vector<Segment> &segments) override;

        private:
            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocateSegmentsBuffer();

            bgfx::DynamicIndexBufferHandle m_DIbh;
            bgfx::DynamicVertexBufferHandle m_DVbh;
            bgfx::DynamicVertexBufferHandle m_SegmentsBuffer;
            
            bgfx::ProgramHandle m_ComputeProgram;

            uint32_t m_SegmentsSize;
    };
}