#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class IndirectBasedPolylines : public GenericLines<PolylinesSettings>
    {
        static const bgfx::ProgramHandle mLinesPH;
        static const bgfx::ProgramHandle mJointPH;
        static const bgfx::ProgramHandle mComputeIndirectPH;

        static const inline std::vector<float> mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t> mIndices = {0, 3, 1, 0, 2, 3};

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::DynamicVertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;
        bgfx::IndirectBufferHandle mSegmentsIndirectBH = BGFX_INVALID_HANDLE;
        bgfx::IndirectBufferHandle mJointIndirectBH = BGFX_INVALID_HANDLE;
        bgfx::UniformHandle mComputeIndirectDataUH = BGFX_INVALID_HANDLE;

    public:
        IndirectBasedPolylines(const std::vector<LinesVertex> &points);

        IndirectBasedPolylines(const IndirectBasedPolylines &other);

        IndirectBasedPolylines(IndirectBasedPolylines &&other);

        ~IndirectBasedPolylines();

        IndirectBasedPolylines &operator=(IndirectBasedPolylines other);

        void swap(IndirectBasedPolylines &other);

        friend void swap(IndirectBasedPolylines &a, IndirectBasedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateIndirectBuffers();

        void allocatePointsBuffer();

        void allocateVerticesBuffer();

        void allocateIndexBuffers();
    };
}
