#pragma once
#include "../generic_lines.hpp"
#include "../polylines_settings.hpp"

namespace lines
{
    class InstancingBasedPolylines : public GenericLines<PolylinesSettings>
    {
        static const bgfx::ProgramHandle mJointPH;
        static const bgfx::ProgramHandle mLinesPH;

        static const inline std::vector<float> mVertices = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        static const inline std::vector<uint32_t> mIndices = {0, 3, 1, 0, 2, 3};

        std::vector<LinesVertex> mPoints;

        mutable bgfx::InstanceDataBuffer mSegmentsInstanceDB;
        mutable bgfx::InstanceDataBuffer mJointInstanceDB;

        bgfx::VertexBufferHandle mVerticesBH = BGFX_INVALID_HANDLE;
        bgfx::IndexBufferHandle mIndicesBH = BGFX_INVALID_HANDLE;

    public:
        InstancingBasedPolylines(const std::vector<LinesVertex> &points);

        InstancingBasedPolylines(const InstancingBasedPolylines &other);

        InstancingBasedPolylines(InstancingBasedPolylines &&other);

        ~InstancingBasedPolylines();

        InstancingBasedPolylines &operator=(InstancingBasedPolylines other);

        void swap(InstancingBasedPolylines &other);

        friend void swap(InstancingBasedPolylines& a, InstancingBasedPolylines& b) { a.swap(b); }

        void draw(uint32_t viewId) const override;

        void update(const std::vector<LinesVertex> &points) override;

    private:
        void generateInstanceBuffer() const;

        void allocateVerticesBuffer();

        void allocateIndexesBuffer();
    };
}