#pragma once
#include <lines.hpp>

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
            void generateBuffers(const std::vector<Segment> &segments);

            bgfx::DynamicIndexBufferHandle m_DIbh;
            bgfx::DynamicVertexBufferHandle m_DVbh;
            bgfx::VertexBufferHandle m_SegmentsBuffer;
            bgfx::ProgramHandle m_ComputeProgram;
    };
}