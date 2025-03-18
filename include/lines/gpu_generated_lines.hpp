#pragma once
#include "../lines.hpp"

namespace lines {
    
    class GPUGeneratedLines : public Lines {

        std::vector<LinesVertex>            mPoints;

        bgfx::DynamicIndexBufferHandle      mIndexesBH              = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mVerticesBH             = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle     mPointsBH               = BGFX_INVALID_HANDLE;  
        bgfx::ProgramHandle                 mComputeVerticesPH      = BGFX_INVALID_HANDLE;

        public:
            GPUGeneratedLines(const std::vector<LinesVertex> &points);

            GPUGeneratedLines(const GPUGeneratedLines& other);

            GPUGeneratedLines(GPUGeneratedLines&& other);
            
            ~GPUGeneratedLines();

            GPUGeneratedLines& operator=(GPUGeneratedLines other);

            void swap(GPUGeneratedLines& other);

            void draw(uint32_t viewId) const override;

            void update(const std::vector<LinesVertex> &points) override;

        private:
            void generateBuffers();

            void allocateVertexBuffer();

            void allocateIndexBuffer();

            void allocatePointsBuffer();
    };
}