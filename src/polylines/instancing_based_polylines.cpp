#include <polylines/instancing_based_polylines.hpp>
#include <cmath>

namespace lines
{
    InstancingBasedPolylines::InstancingBasedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        checkCaps();
        allocateVerticesAndIndicesBuffers();
    }

    InstancingBasedPolylines::~InstancingBasedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }

    void InstancingBasedPolylines::swap(InstancingBasedPolylines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        swap(mPoints, other.mPoints);

        swap(mSegmentsInstanceDB, other.mSegmentsInstanceDB);
        swap(mJointInstanceDB, other.mJointInstanceDB);

        swap(mVerticesBH, other.mVerticesBH);
        swap(mIndexesBH, other.mIndexesBH);

        swap(mLinesPH, other.mLinesPH);
    }

    void InstancingBasedPolylines::draw(uint32_t viewId) const
    {
        generateInstanceDataBuffer();
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setInstanceDataBuffer(&mSegmentsInstanceDB);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndexesBH);
            bgfx::setInstanceDataBuffer(&mJointInstanceDB);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJointPH);
        }
    }

    void InstancingBasedPolylines::update(const std::vector<LinesVertex> &points)
    {
        mPoints = points;
    }

    void InstancingBasedPolylines::generateInstanceDataBuffer() const
    {
        const uint16_t strideSegments = sizeof(float) * 20;
        uint32_t linesNumSegments = bgfx::getAvailInstanceDataBuffer(mPoints.size() - 1, strideSegments);
        bgfx::allocInstanceDataBuffer(&mSegmentsInstanceDB, linesNumSegments, strideSegments);

        const uint16_t strideJoints = sizeof(float) * 16;
        if (mPoints.size() > 2)
        {
            uint32_t linesNumJoints = bgfx::getAvailInstanceDataBuffer(mPoints.size() - 2, strideJoints);
            bgfx::allocInstanceDataBuffer(&mJointInstanceDB, linesNumJoints, strideJoints);
        }

        uint8_t *dataSegments = mSegmentsInstanceDB.data;
        uint8_t *dataJoints = mJointInstanceDB.data;

        for (uint32_t i = 0; i < linesNumSegments; i++) {
            float* prevSegments = reinterpret_cast<float*>(dataSegments);
            prevSegments[0]     = mPoints[i - !!i].X;
            prevSegments[1]     = mPoints[i - !!i].Y;
            prevSegments[2]     = mPoints[i - !!i].Z;
            prevSegments[3]     = mPoints[i].xN;
    
            float* currSegments = reinterpret_cast<float*>(&dataSegments[16]);
            currSegments[0]     = mPoints[i].X;
            currSegments[1]     = mPoints[i].Y;
            currSegments[2]     = mPoints[i].Z;
    
            uint32_t* color0 = reinterpret_cast<uint32_t*>(&dataSegments[28]);
            color0[0]        = mPoints[i].getRGBAColor();
    
            float* nextSegments = reinterpret_cast<float*>(&dataSegments[32]);
            nextSegments[0]     = mPoints[i + 1].X;
            nextSegments[1]     = mPoints[i + 1].Y;
            nextSegments[2]     = mPoints[i + 1].Z;
    
            uint32_t* color1 = reinterpret_cast<uint32_t*>(&dataSegments[44]);
            color1[0]        = mPoints[i + 1].getRGBAColor();
    
            float* next_nextSegments = reinterpret_cast<float*>(&dataSegments[48]);
            next_nextSegments[0] =
                mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].X;
            next_nextSegments[1] =
                mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].Y;
            next_nextSegments[2] =
                mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].Z;
            next_nextSegments[3] = mPoints[i].yN;
    
            float* normalSegments = reinterpret_cast<float*>(&dataSegments[64]);
            normalSegments[0]     = mPoints[i].zN;
            normalSegments[1]     = mPoints[i + 1].xN;
            normalSegments[2]     = mPoints[i + 1].yN;
            normalSegments[3]     = mPoints[i + 1].zN;
    
            if (i > 0) {
                float* prevJoint = reinterpret_cast<float*>(dataJoints);
                prevJoint[0]     = mPoints[i - 1].X;
                prevJoint[1]     = mPoints[i - 1].Y;
                prevJoint[2]     = mPoints[i - 1].Z;
                prevJoint[3]     = 0.0f;
    
                float* currJoint = reinterpret_cast<float*>(&dataJoints[16]);
                currJoint[0]     = mPoints[i].X;
                currJoint[1]     = mPoints[i].Y;
                currJoint[2]     = mPoints[i].Z;
    
                uint32_t* colorJoint = reinterpret_cast<uint32_t*>(&dataJoints[28]);
                colorJoint[0]        = mPoints[i].getRGBAColor();
    
                float* nextJoint = reinterpret_cast<float*>(&dataJoints[32]);
                nextJoint[0]     = mPoints[i + 1].X;
                nextJoint[1]     = mPoints[i + 1].Y;
                nextJoint[2]     = mPoints[i + 1].Z;
                nextJoint[3]     = 0.0f;
    
                float* normalJoint = reinterpret_cast<float*>(&dataJoints[48]);
                normalJoint[0]     = mPoints[i].xN;
                normalJoint[1]     = mPoints[i].yN;
                normalJoint[2]     = mPoints[i].zN;
                normalJoint[3]     = 0;
    
                dataJoints += strideJoints;
            }
    
            dataSegments += strideSegments;
        }
    }

    void InstancingBasedPolylines::allocateVerticesAndIndicesBuffers()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(&VERTICES[0], sizeof(float) * VERTICES.size()),
            layout);

        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&INDICES[0], sizeof(uint32_t) * INDICES.size()),
            BGFX_BUFFER_INDEX32);
    }
}