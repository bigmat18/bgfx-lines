#pragma once
#include "../polylines.hpp"

namespace lines {
    class GPUGeneratedPolylines : public Polylines {
        public:
            GPUGeneratedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth);

            ~GPUGeneratedPolylines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<GPUGeneratedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();

            bgfx::DynamicVertexBufferHandle m_DVbh;
            bgfx::DynamicIndexBufferHandle m_SegmentsDIbh;
            bgfx::DynamicIndexBufferHandle m_JoinsDIbh;

            bgfx::DynamicVertexBufferHandle m_PointsBuffer;
            
            bgfx::ProgramHandle m_ComputeProgram;
            bgfx::UniformHandle m_NumWorksGroupUniform;

            uint32_t m_PointsSize;
    };
}