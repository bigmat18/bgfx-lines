#include <polylines/indirect_based_polylines.hpp>

namespace lines
{
    const bgfx::ProgramHandle IndirectBasedPolylines::mLinesPH = LoadProgram(
        "polylines/indirect_based_polylines/vs_indirect_based_segments",
        "polylines/indirect_based_polylines/fs_indirect_based_polylines");

    const bgfx::ProgramHandle IndirectBasedPolylines::mComputeIndirectPH = bgfx::createProgram(
        LoadShader("polylines/indirect_based_polylines/cs_compute_indirect"), true);

    const bgfx::ProgramHandle IndirectBasedPolylines::mJointPH = LoadProgram(
        "polylines/indirect_based_polylines/vs_indirect_based_joins",
        "polylines/indirect_based_polylines/fs_indirect_based_polylines");

    IndirectBasedPolylines::IndirectBasedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points),
        mJointIndirectBH(bgfx::createIndirectBuffer(1)),
        mSegmentsIndirectBH(bgfx::createIndirectBuffer(1)),
        mComputeIndirectDataUH(bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
    {
        allocateIndexBuffers();
        allocateVerticesBuffer();
        generateIndirectBuffers();

        allocatePointsBuffer();
        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    IndirectBasedPolylines::IndirectBasedPolylines(const IndirectBasedPolylines &other) : 
        mPoints(other.mPoints),
        mJointIndirectBH(bgfx::createIndirectBuffer(1)),
        mSegmentsIndirectBH(bgfx::createIndirectBuffer(1)),
        mComputeIndirectDataUH(bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
    {
        allocateIndexBuffers();
        allocateVerticesBuffer();
        generateIndirectBuffers();

        allocatePointsBuffer();
        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    IndirectBasedPolylines::IndirectBasedPolylines(IndirectBasedPolylines &&other)
    {
        swap(other);
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
    }

    IndirectBasedPolylines &IndirectBasedPolylines::operator=(IndirectBasedPolylines other)
    {
        swap(other);
        return *this;
    }

    void IndirectBasedPolylines::swap(IndirectBasedPolylines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        std::swap(mPoints, other.mPoints);
        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndicesBH, other.mIndicesBH);
        std::swap(mPointsBH, other.mPointsBH);
        std::swap(mSegmentsIndirectBH, other.mSegmentsIndirectBH);
        std::swap(mJointIndirectBH, other.mJointIndirectBH);
        std::swap(mComputeIndirectDataUH, other.mComputeIndirectDataUH);
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

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
    }

    void IndirectBasedPolylines::generateIndirectBuffers()
    {
        float data[] = {static_cast<float>(mPoints.size()), 0, 0, 0};
        bgfx::setUniform(mComputeIndirectDataUH, data);

        bgfx::setBuffer(0, mSegmentsIndirectBH, bgfx::Access::Write);
        bgfx::setBuffer(1, mJointIndirectBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeIndirectPH);
    }

    void IndirectBasedPolylines::allocatePointsBuffer()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        mPointsBH = bgfx::createDynamicVertexBuffer(mPoints.size(), layout,
                                                    BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
    }

    void IndirectBasedPolylines::allocateVerticesBuffer()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()),
            layout);
    }

    void IndirectBasedPolylines::allocateIndexBuffers()
    {
        mIndicesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndices[0], sizeof(uint32_t) * mIndices.size()),
            BGFX_BUFFER_INDEX32);
    }

}