#include <polylines/gpu_generated_polylines.hpp>

namespace lines
{
    const bgfx::ProgramHandle GPUGeneratedPolylines::mLinesPH = LoadProgram(
        "polylines/cpu_generated_polylines/vs_cpu_generated_polylines",
        "polylines/cpu_generated_polylines/fs_cpu_generated_polylines");

    const bgfx::ProgramHandle GPUGeneratedPolylines::mComputeVertexPH = bgfx::createProgram(
        LoadShader("polylines/gpu_generated_polylines/cs_compute_buffers"), true);


    GPUGeneratedPolylines::GPUGeneratedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points),
        mComputeDataUH(bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4))
    {
        allocatePointsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    GPUGeneratedPolylines::GPUGeneratedPolylines(const GPUGeneratedPolylines &other) : 
        mPoints(other.mPoints),
        mComputeDataUH(bgfx::createUniform("u_numWorksGroups", bgfx::UniformType::Vec4))
    {
        allocatePointsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    GPUGeneratedPolylines::GPUGeneratedPolylines(GPUGeneratedPolylines &&other)
    {
        swap(other);
    }

    GPUGeneratedPolylines::~GPUGeneratedPolylines()
    {
        if (bgfx::isValid(mVertexBH))
            bgfx::destroy(mVertexBH);

        if (bgfx::isValid(mSegmentsIndicesBH))
            bgfx::destroy(mSegmentsIndicesBH);

        if (bgfx::isValid(mJointIndicesBH))
            bgfx::destroy(mJointIndicesBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if (bgfx::isValid(mComputeDataUH))
            bgfx::destroy(mComputeDataUH);
    }

    GPUGeneratedPolylines &GPUGeneratedPolylines::operator=(GPUGeneratedPolylines other)
    {
        swap(other);
        return *this;
    }

    void GPUGeneratedPolylines::swap(GPUGeneratedPolylines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        std::swap(mPoints, other.mPoints);

        std::swap(mPointsBH, other.mPointsBH);
        std::swap(mVertexBH, other.mVertexBH);

        std::swap(mSegmentsIndicesBH, other.mSegmentsIndicesBH);
        std::swap(mJointIndicesBH, other.mJointIndicesBH);

        std::swap(mComputeDataUH, other.mComputeDataUH);
    }

    void GPUGeneratedPolylines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVertexBH);
        bgfx::setIndexBuffer(mSegmentsIndicesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (bgfx::isValid(mJointIndicesBH) && settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVertexBH);
            bgfx::setIndexBuffer(mJointIndicesBH);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mLinesPH);
        }
    }

    void GPUGeneratedPolylines::update(const std::vector<LinesVertex> &points)
    {
        int oldSize = mPoints.size();
        mPoints = points;

        if (oldSize != mPoints.size())
        {
            bgfx::destroy(mSegmentsIndicesBH);
            bgfx::destroy(mJointIndicesBH);
            allocateIndexBuffer();
        }

        if (oldSize < mPoints.size())
        {
            bgfx::destroy(mVertexBH);
            allocateVertexBuffer();
        }

        if (oldSize > mPoints.size())
        {
            bgfx::destroy(mPointsBH);
            allocatePointsBuffer();
        }

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    void GPUGeneratedPolylines::generateBuffers()
    {
        float data[] = {static_cast<float>(mPoints.size() - 1), 0, 0, 0};
        bgfx::setUniform(mComputeDataUH, data);

        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setBuffer(1, mVertexBH, bgfx::Access::Write);
        bgfx::setBuffer(2, mSegmentsIndicesBH, bgfx::Access::Write);
        bgfx::setBuffer(3, mJointIndicesBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeVertexPH, mPoints.size() - 1, 1, 1);
    }

    void GPUGeneratedPolylines::allocateVertexBuffer()
    {

        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        mVertexBH = bgfx::createDynamicVertexBuffer(
            (mPoints.size() - 1) * 4, layout,
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE);
    }

    void GPUGeneratedPolylines::allocateIndexBuffer()
    {
        mSegmentsIndicesBH = bgfx::createDynamicIndexBuffer(
            ((mPoints.size() - 1) * 6),
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);

        mJointIndicesBH = bgfx::createDynamicIndexBuffer(
            ((mPoints.size() - 2) * 6),
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }

    void GPUGeneratedPolylines::allocatePointsBuffer()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .end();

        mPointsBH = bgfx::createDynamicVertexBuffer(
            mPoints.size(), layout,
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
    }
}