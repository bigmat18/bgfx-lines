#pragma once 
#include "../lines.hpp"

namespace lines {
    class InstancingBasedLines : public Lines {

        std::vector<float>          mVertices = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };
        std::vector<uint32_t>       mIndexes = { 0, 1, 2, 1, 3, 2 };
        std::vector<LinesVertex>    mPoints;

        bgfx::VertexBufferHandle    mVerticesBH         = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle     mIndexesBH          = BGFX_INVALID_HANDLE;
        bgfx::InstanceDataBuffer    mInstanceDB;

        public:
            InstancingBasedLines(const std::vector<LinesVertex> &points);

            InstancingBasedLines(const InstancingBasedLines& other);

            InstancingBasedLines(InstancingBasedLines&& other);

            ~InstancingBasedLines();

            InstancingBasedLines& operator=(InstancingBasedLines other);

            void swap(InstancingBasedLines& other);

            std::shared_ptr<vcl::DrawableObject> clone() const override;

            void draw(uint viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:

            void generateInstanceDataBuffer();

            void allocateVerticesBuffer();

            void allocateIndexesBuffer();
    }; 
}