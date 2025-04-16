#include <polylines/indirect_based_polylines.hpp>

namespace lines
{

    IndirectBasedPolylines::IndirectBasedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        allocateVerticesAndIndicesBuffers();
        generateIndirectBuffers();
        allocateAndSetPointsBuffer();
    }

    IndirectBasedPolylines::~IndirectBasedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndicesBH))
            bgfx::destroy(mIndicesBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if (bgfx::isValid(mSegmentsIndirectBH))
            bgfx::destroy(mSegmentsIndirectBH);

        if (bgfx::isValid(mJointIndirectBH))
            bgfx::destroy(mJointIndirectBH);

        if (bgfx::isValid(mComputeIndirectDataUH))
            bgfx::destroy(mComputeIndirectDataUH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);

        if (bgfx::isValid(mComputeIndirectPH))
            bgfx::destroy(mComputeIndirectPH);
    }

    void IndirectBasedPolylines::swap(IndirectBasedPolylines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        swap(mPoints, other.mPoints);
        swap(mVerticesBH, other.mVerticesBH);
        swap(mIndicesBH, other.mIndicesBH);
        swap(mPointsBH, other.mPointsBH);

        swap(mSegmentsIndirectBH, other.mSegmentsIndirectBH);
        swap(mJointIndirectBH, other.mJointIndirectBH);
        swap(mComputeIndirectDataUH, other.mComputeIndirectDataUH);

        swap(mLinesPH, other.mLinesPH);
        swap(mComputeIndirectPH, other.mComputeIndirectPH);
    }

    void IndirectBasedPolylines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        float indirectData[] = {static_cast<float>(mPoints.size() - 1), 0, 0, 0};
        bgfx::setUniform(mComputeIndirectDataUH, indirectData);

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndicesBH);
        bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH, mSegmentsIndirectBH, 0);

        if (settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndicesBH);
            bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJointPH, mJointIndirectBH, 0);
        }
    }

    void IndirectBasedPolylines::update(const std::vector<LinesVertex> &points)
    {
        int oldSize = mPoints.size();
        mPoints = points;

        if (oldSize != mPoints.size())
        {
            generateIndirectBuffers();
        }

        allocateAndSetPointsBuffer();
    }

    void IndirectBasedPolylines::generateIndirectBuffers()
    {
        float data[] = {static_cast<float>(mPoints.size()), 0, 0, 0};
        bgfx::setUniform(mComputeIndirectDataUH, data);

        bgfx::setBuffer(0, mSegmentsIndirectBH, bgfx::Access::Write);
        bgfx::setBuffer(1, mJointIndirectBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeIndirectPH);
    }

    void IndirectBasedPolylines::allocateAndSetPointsBuffer()
    {

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<LinesVertex>(mPoints.size());

        std::copy(mPoints.begin(), mPoints.end(), buffer);

        mPointsBH = bgfx::createVertexBuffer(
            bgfx::makeRef(buffer, sizeof(LinesVertex) * mPoints.size(), releaseFn),
            layout, BGFX_BUFFER_COMPUTE_READ);
    }

    void IndirectBasedPolylines::allocateVerticesAndIndicesBuffers()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(&VERTICES[0], sizeof(float) * VERTICES.size()),
            layout);

        mIndicesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&INDICES[0], sizeof(uint32_t) * INDICES.size()),
            BGFX_BUFFER_INDEX32);
    }

}