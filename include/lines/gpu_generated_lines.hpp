#pragma once
#include "../lines.hpp"

namespace lines {
    class GPUGeneratedLines : public Lines {

        public:
            GPUGeneratedLines(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth);

            ~GPUGeneratedLines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<GPUGeneratedLines>(*this);
            };

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:
            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();

            bgfx::DynamicIndexBufferHandle m_DIbh;
            bgfx::DynamicVertexBufferHandle m_DVbh;
            bgfx::DynamicVertexBufferHandle m_PointsBuffer;
            
            bgfx::ProgramHandle m_ComputeProgram;

            uint32_t m_PointsSize;
    };
}