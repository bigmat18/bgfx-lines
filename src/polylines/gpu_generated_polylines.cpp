#include <polylines/gpu_generated_polylines.hpp>

namespace lines
{

    GPUGeneratedPolylines::GPUGeneratedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        checkCaps();
        
        if (points.size() > 1) {
            allocateAndSetPointsBuffer();
            allocateVertexBuffer();
            allocateIndexBuffer();
            generateVerticesAndIndicesBuffers();
        } else {
            throw std::runtime_error("Points.size() must be > 1");
        }
    }

    GPUGeneratedPolylines::~GPUGeneratedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mSegmentIndicesBH))
            bgfx::destroy(mSegmentIndicesBH);

        if (bgfx::isValid(mJointIndicesBH))
            bgfx::destroy(mJointIndicesBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if (bgfx::isValid(mComputeDataUH))
            bgfx::destroy(mComputeDataUH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);

        if (bgfx::isValid(mComputeVertexPH))
            bgfx::destroy(mComputeVertexPH);
    }

    void GPUGeneratedPolylines::swap(GPUGeneratedPolylines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        swap(mPoints, other.mPoints);

        swap(mPointsBH, other.mPointsBH);
        swap(mVerticesBH, other.mVerticesBH);

        swap(mSegmentIndicesBH, other.mSegmentIndicesBH);
        swap(mJointIndicesBH, other.mJointIndicesBH);

        swap(mComputeDataUH, other.mComputeDataUH);

        swap(mLinesPH, other.mLinesPH);
        swap(mComputeVertexPH, other.mComputeVertexPH);
    }

    void GPUGeneratedPolylines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mSegmentIndicesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (bgfx::isValid(mJointIndicesBH) && settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mJointIndicesBH);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mLinesPH);
        }
    }

    void GPUGeneratedPolylines::update(const std::vector<LinesVertex> &points)
    {
        mPoints = points;
        bgfx::destroy(mVerticesBH);
        bgfx::destroy(mSegmentIndicesBH);
        bgfx::destroy(mJointIndicesBH);

        if (points.size() > 1) {
            allocateAndSetPointsBuffer();
            allocateVertexBuffer();
            allocateIndexBuffer();
            generateVerticesAndIndicesBuffers();
        } else {
            throw std::runtime_error("Points.size() must be > 1");
        }
    }

    void GPUGeneratedPolylines::generateVerticesAndIndicesBuffers()
    {
        float data[] = {static_cast<float>(mPoints.size() - 1), 0, 0, 0};
        bgfx::setUniform(mComputeDataUH, data);

        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setBuffer(1, mVerticesBH, bgfx::Access::Write);
        bgfx::setBuffer(2, mSegmentIndicesBH, bgfx::Access::Write);
        bgfx::setBuffer(3, mJointIndicesBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeVertexPH, mPoints.size() - 1, 1, 1);
    }

    void GPUGeneratedPolylines::allocateVertexBuffer()
    {

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 3, bgfx::AttribType::Float)
            .end();
    
        const uint32_t sz = (mPoints.size() - 1) * 4 * 16;
    
        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(nullptr, sizeof(float) * sz),
            layout, BGFX_BUFFER_COMPUTE_WRITE);
    }

    void GPUGeneratedPolylines::allocateIndexBuffer()
    {
        const uint32_t szSeg   = (mPoints.size() - 1) * 6;
        const uint32_t szJoint = (mPoints.size() - 2) * 6;
    
        mSegmentIndicesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(nullptr, sizeof(uint32_t) * szSeg),
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32);
    
        if (mPoints.size() > 2) {
            mJointIndicesBH = bgfx::createIndexBuffer(
                bgfx::makeRef(nullptr, sizeof(uint32_t) * szJoint),
                BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32);
        }
    }

    void GPUGeneratedPolylines::allocateAndSetPointsBuffer()
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .end();
    
        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<LinesVertex>(mPoints.size());
    
        std::copy(mPoints.begin(), mPoints.end(), buffer);
    
        mPointsBH = bgfx::createVertexBuffer(
            bgfx::makeRef(buffer, sizeof(LinesVertex) * mPoints.size(), releaseFn),
            layout, BGFX_BUFFER_COMPUTE_READ);
    }
}