#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class GPUGeneratedPolylines : public GenericLines<PolylinesSettings>
    {
        bgfx::ProgramHandle mComputeVertexPH = bgfx::createProgram(
            LoadShader("polylines/gpu_generated_polylines/cs_compute_buffers"), true);

        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "polylines/cpu_generated_polylines/vs_cpu_generated_polylines",
            "polylines/cpu_generated_polylines/fs_cpu_generated_polylines");

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;

        bgfx::IndexBufferHandle mSegmentIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mJointIndicesBH = BGFX_INVALID_HANDLE;

        bgfx::UniformHandle mComputeDataUH = bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4);

    public:
        GPUGeneratedPolylines() { checkCaps(); }

        GPUGeneratedPolylines(const std::vector<LinesVertex> &points);

        GPUGeneratedPolylines(const GPUGeneratedPolylines &other) = delete;

        GPUGeneratedPolylines(GPUGeneratedPolylines &&other) = delete;

        GPUGeneratedPolylines &operator=(GPUGeneratedPolylines other) = delete;
        
        ~GPUGeneratedPolylines();

        void swap(GPUGeneratedPolylines &other);

        friend void swap(GPUGeneratedPolylines& a, GPUGeneratedPolylines& b) { a.swap(b); }

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

        void allocateAndSetPointsBuffer();

        void allocateVertexBuffer();
    
        void allocateIndexBuffer();
    
        void generateVerticesAndIndicesBuffers();
    };
}