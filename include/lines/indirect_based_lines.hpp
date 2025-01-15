#pragma once
#include "../lines.hpp"

namespace lines {
    class IndirectBasedLines : public Lines {

        std::vector<float>                  mVertices = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        std::vector<uint32_t>               mIndexes = {0, 1, 2, 1, 3, 2};
        std::vector<LinesVertex>            mPoints;

        bgfx::VertexBufferHandle            mVerticesBH;
        bgfx::IndexBufferHandle             mIndexesBH;
        bgfx::DynamicVertexBufferHandle     mPointsBH;

        bgfx::IndirectBufferHandle          mIndirectBH;
        bgfx::ProgramHandle                 mComputeIndirectPH;            
        bgfx::UniformHandle                 mIndirectDataUH;

        public:
            IndirectBasedLines(const std::vector<LinesVertex> &points);

            IndirectBasedLines(const IndirectBasedLines& other);

            IndirectBasedLines(IndirectBasedLines&& other);

            ~IndirectBasedLines();

            IndirectBasedLines& operator=(IndirectBasedLines other);

            void swap(IndirectBasedLines& other);

            std::shared_ptr<vcl::DrawableObjectI> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void allocatePointsBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();

            void generateIndirectBuffer();
    };  
}