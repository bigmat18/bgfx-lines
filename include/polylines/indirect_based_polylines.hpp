#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class IndirectBasedPolylines : public GenericLines<PolylinesSettings>
    {
        static const inline std::vector<float> VERTICES =
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t> INDICES = {0, 3, 1, 0, 2, 3};

        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "polylines/indirect_based_polylines/vs_indirect_based_segments",
            "polylines/indirect_based_polylines/fs_indirect_based_polylines");

        bgfx::ProgramHandle mJointPH = LoadProgram(
            "polylines/indirect_based_polylines/vs_indirect_based_joins",
            "polylines/indirect_based_polylines/fs_indirect_based_polylines");

        bgfx::ProgramHandle mComputeIndirectPH = bgfx::createProgram(
            LoadShader("polylines/indirect_based_polylines/cs_compute_indirect"), true);

        std::vector<LinesVertex> mPoints;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndicesBH = BGFX_INVALID_HANDLE;
        bgfx::VertexBufferHandle mPointsBH = BGFX_INVALID_HANDLE;

        bgfx::IndirectBufferHandle mSegmentsIndirectBH = bgfx::createIndirectBuffer(1);
        bgfx::IndirectBufferHandle mJointIndirectBH = bgfx::createIndirectBuffer(1);
        bgfx::UniformHandle mComputeIndirectDataUH = bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4);

    public:
        IndirectBasedPolylines() { checkCaps(); }

        IndirectBasedPolylines(const std::vector<LinesVertex> &points);

        IndirectBasedPolylines(const IndirectBasedPolylines &other) = delete;

        IndirectBasedPolylines(IndirectBasedPolylines &&other) = delete;

        ~IndirectBasedPolylines();

        IndirectBasedPolylines &operator=(IndirectBasedPolylines other) = delete;

        void swap(IndirectBasedPolylines &other);

        friend void swap(IndirectBasedPolylines &a, IndirectBasedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps* caps      = bgfx::getCaps();
            const bool computeSupported = bool(caps->supported & BGFX_CAPS_COMPUTE);
            const bool indirectSupported =
                bool(caps->supported & BGFX_CAPS_DRAW_INDIRECT);
            const bool instancingSupported =
                bool(caps->supported & BGFX_CAPS_INSTANCING);

            if (!(instancingSupported && computeSupported && indirectSupported)) {
                throw std::runtime_error("Instancing or compute are not supported");
            }
        }

        void generateIndirectBuffers();

        void allocateAndSetPointsBuffer();

        void allocateVerticesAndIndicesBuffers();
    };
}
