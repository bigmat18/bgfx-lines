#pragma once
#include <lines.hpp>

namespace lines {
    class GPUGeneratedLines : public Lines {

        public:
            GPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth);

            ~GPUGeneratedLines() = default;

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<GPUGeneratedLines>(*this);
            };

            void draw(uint viewId) const override;

        private:
            bgfx::DynamicIndexBufferHandle m_DIbh;
            bgfx::DynamicVertexBufferHandle m_DVbh;
            bgfx::DynamicVertexBufferHandle m_PointsBuffer;
            bgfx::ProgramHandle m_ComputeProgram;
    };
}