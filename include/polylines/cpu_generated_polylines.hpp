#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class CPUGeneratedPolylines : public GenericLines<PolylinesSettings>
    {
        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "polylines/cpu_generated_polylines/vs_cpu_generated_polylines",
            "polylines/cpu_generated_polylines/fs_cpu_generated_polylines");

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mSegmentIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mJointIndicesBH = BGFX_INVALID_HANDLE;

    public:
        CPUGeneratedPolylines() = default;

        CPUGeneratedPolylines(const std::vector<LinesVertex> &points);

        CPUGeneratedPolylines(const CPUGeneratedPolylines &other) = delete;

        CPUGeneratedPolylines(CPUGeneratedPolylines &&other) = delete;

        CPUGeneratedPolylines &operator=(CPUGeneratedPolylines other) = delete;

        ~CPUGeneratedPolylines();

        void swap(CPUGeneratedPolylines &other);

        friend void swap(CPUGeneratedPolylines& a, CPUGeneratedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateVerticesAndIndicesBuffers(const std::vector<LinesVertex> &points);
    };
}