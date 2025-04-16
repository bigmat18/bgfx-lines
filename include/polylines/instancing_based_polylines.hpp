#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class InstancingBasedPolylines : public GenericLines<PolylinesSettings>
    {
        static const inline std::vector<float> VERTICES =
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t> INDICES = {0, 3, 1, 0, 2, 3};

        bgfx::ProgramHandle mJointPH = LoadProgram(
            "polylines/instancing_based_polylines/vs_instancing_based_joins",
            "polylines/instancing_based_polylines/fs_instancing_based_polylines");

        bgfx::ProgramHandle mLinesPH = LoadProgram(
            "polylines/instancing_based_polylines/vs_instancing_based_segments",
            "polylines/instancing_based_polylines/fs_instancing_based_polylines");

        std::vector<LinesVertex> mPoints;

        mutable bgfx::InstanceDataBuffer mSegmentsInstanceDB;
        mutable bgfx::InstanceDataBuffer mJointInstanceDB;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndexesBH = BGFX_INVALID_HANDLE;

    public:
        InstancingBasedPolylines() { checkCaps(); }

        InstancingBasedPolylines(const std::vector<LinesVertex> &points);

        InstancingBasedPolylines(const InstancingBasedPolylines &other) = delete;

        InstancingBasedPolylines(InstancingBasedPolylines &&other) = delete;

        ~InstancingBasedPolylines();

        InstancingBasedPolylines &operator=(InstancingBasedPolylines other) = delete;

        void swap(InstancingBasedPolylines &other);

        friend void swap(InstancingBasedPolylines& a, InstancingBasedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void checkCaps() const
        {
            const bgfx::Caps* caps = bgfx::getCaps();
            const bool        instancingSupported =
                bool(caps->supported & BGFX_CAPS_INSTANCING);

            if (!instancingSupported) {
                throw std::runtime_error("Instancing or compute are not supported");
            }
        }

        void generateInstanceDataBuffer() const;

        void allocateVerticesAndIndicesBuffers();
    };
}