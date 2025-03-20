#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class GPUGeneratedLines : public GenericLines
    {

        static bgfx::ProgramHandle mLinesPH = LoadProgram("lines/cpu_generated_lines/vs_cpu_generated_lines", 
                                                          "lines/cpu_generated_lines/fs_cpu_generated_lines");
        static bgfx::ProgramHandle mComputeVerticesPH = bgfx::createProgram(
                LoadShader("lines/gpu_generated_lines/cs_compute_buffers"), true)
        
        std::vector<LinesVertex> mPoints;

        bgfx::DynamicVertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicIndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;

    public: 
        GPUGeneratedLines(const std::vector<LinesVertex> &points);

        GPUGeneratedLines(const GPUGeneratedLines &other);

        GPUGeneratedLines(GPUGeneratedLines &&other);

        ~GPUGeneratedLines();

        GPUGeneratedLines &operator=(GPUGeneratedLines other);

        void swap(GPUGeneratedLines &other);

        friend void swap(GPUGeneratedLines& a, GPUGeneratedLines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateBuffers();

        void allocateVertexBuffer();

        void allocateIndexBuffer();

        void allocatePointsBuffer();
    };
}