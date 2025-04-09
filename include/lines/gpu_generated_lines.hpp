#pragma once
#include "../generic_lines.hpp"

namespace lines
{
    class GPUGeneratedLines : public GenericLines<LinesSettings>
    {

        bgfx::ProgramHandle mLinesPH  = LoadProgram(
            "lines/cpu_generated_lines/vs_cpu_generated_lines",
            "lines/cpu_generated_lines/fs_cpu_generated_lines");
            
        bgfx::ProgramHandle mComputeVerticesPH = bgfx::createProgram(
            LoadShader("lines/gpu_generated_lines/cs_compute_buffers"), true);

        uint32_t mPointsSize;
        bgfx::VertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;

    public: 
        GPUGeneratedLines() { checkCaps(); };

        GPUGeneratedLines(const std::vector<LinesVertex> &points);

        GPUGeneratedLines(const GPUGeneratedLines &other) = delete;

        GPUGeneratedLines(GPUGeneratedLines &&other) = delete;
        
        GPUGeneratedLines &operator=(GPUGeneratedLines other) = delete;
        
        ~GPUGeneratedLines();

        void swap(GPUGeneratedLines &other);

        friend void swap(GPUGeneratedLines& a, GPUGeneratedLines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps* caps = bgfx::getCaps();
            const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
            if (!computeSupported) {
                throw std::runtime_error("GPU compute not supported");
            }
        }

        void allocateAndSetPointsBuffer(const std::vector<LinesVertex>& points);

        void allocateVertexBuffer();

        void allocateIndexBuffer();

        void generateVerticesAndIndicesBuffers();
    };
}