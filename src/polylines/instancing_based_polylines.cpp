#include <polylines/instancing_based_polylines.hpp>
#include <cmath>

namespace lines
{
    const bgfx::ProgramHandle InstancingBasedPolylines::mJointPH = LoadProgram(
        "polylines/instancing_based_polylines/vs_instancing_based_segments",
        "polylines/instancing_based_polylines/fs_instancing_based_polylines");

    const bgfx::ProgramHandle InstancingBasedPolylines::mLinesPH = LoadProgram(
        "polylines/instancing_based_polylines/vs_instancing_based_joins",
        "polylines/instancing_based_polylines/fs_instancing_based_polylines");

    InstancingBasedPolylines::InstancingBasedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
    }

    InstancingBasedPolylines::InstancingBasedPolylines(const InstancingBasedPolylines &other) : 
        mPoints(other.mPoints)
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
    }

    InstancingBasedPolylines::InstancingBasedPolylines(InstancingBasedPolylines &&other)
    {
        swap(other);
    }

    InstancingBasedPolylines::~InstancingBasedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndicesBH))
            bgfx::destroy(mIndicesBH);
    }

    InstancingBasedPolylines &InstancingBasedPolylines::operator=(InstancingBasedPolylines other)
    {
        swap(other);
        return *this;
    }

    void InstancingBasedPolylines::swap(InstancingBasedPolylines &other)
    {
        std::swap(mPoints, other.mPoints);

        std::swap(mSegmentsInstanceDB, other.mSegmentsInstanceDB);
        std::swap(mJointInstanceDB, other.mJointInstanceDB);

        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndicesBH, other.mIndicesBH);
    }

    void InstancingBasedPolylines::draw(uint32_t viewId) const
    {
        generateInstanceBuffer();
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndicesBH);
        bgfx::setInstanceDataBuffer(&mSegmentsInstanceDB);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndicesBH);
            bgfx::setInstanceDataBuffer(&mJointInstanceDB);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJointPH);
        }
    }

    void InstancingBasedPolylines::update(const std::vector<LinesVertex> &points)
    {
        mPoints = points;
    }

    void InstancingBasedPolylines::generateInstanceBuffer() const
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

        for (uint32_t i = 0; i < linesNumSegments; i++)
        {
            float *prevSegments = reinterpret_cast<float *>(dataSegments);
            prevSegments[0] = mPoints[i - !!i].X;
            prevSegments[1] = mPoints[i - !!i].Y;
            prevSegments[2] = mPoints[i - !!i].Z;
            prevSegments[3] = mPoints[i].xN;

            float *currSegments = (float *)&dataSegments[16];
            currSegments[0] = mPoints[i].X;
            currSegments[1] = mPoints[i].Y;
            currSegments[2] = mPoints[i].Z;

            uint32_t *color0 = (uint32_t *)&dataSegments[28];
            color0[0] = mPoints[i].getABGRColor();

            float *nextSegments = (float *)&dataSegments[32];
            nextSegments[0] = mPoints[i + 1].X;
            nextSegments[1] = mPoints[i + 1].Y;
            nextSegments[2] = mPoints[i + 1].Z;

            uint32_t *color1 = (uint32_t *)&dataSegments[44];
            color1[0] = mPoints[i + 1].getABGRColor();

            float *next_nextSegments = (float *)&dataSegments[48];
            next_nextSegments[0] = mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].X;
            next_nextSegments[1] = mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].Y;
            next_nextSegments[2] = mPoints[i + 1 + (!!(linesNumSegments - 1 - i))].Z;
            next_nextSegments[3] = mPoints[i].yN;

            float *normalSegments = (float *)&dataSegments[64];
            normalSegments[0] = mPoints[i].zN;
            normalSegments[1] = mPoints[i + 1].xN;
            normalSegments[2] = mPoints[i + 1].yN;
            normalSegments[3] = mPoints[i + 1].zN;

            if (i > 0)
            {
                float *prevJoin = reinterpret_cast<float *>(dataJoints);
                prevJoin[0] = mPoints[i - 1].X;
                prevJoin[1] = mPoints[i - 1].Y;
                prevJoin[2] = mPoints[i - 1].Z;
                prevJoin[3] = 0.0f;

                float *currJoin = (float *)&dataJoints[16];
                currJoin[0] = mPoints[i].X;
                currJoin[1] = mPoints[i].Y;
                currJoin[2] = mPoints[i].Z;

                uint32_t *colorJoin = (uint32_t *)&dataJoints[28];
                colorJoin[0] = mPoints[i].getABGRColor();

                float *nextJoin = (float *)&dataJoints[32];
                nextJoin[0] = mPoints[i + 1].X;
                nextJoin[1] = mPoints[i + 1].Y;
                nextJoin[2] = mPoints[i + 1].Z;
                nextJoin[3] = 0.0f;

                float *normalJoin = (float *)&dataJoints[48];
                normalJoin[0] = mPoints[i].xN;
                normalJoin[1] = mPoints[i].yN;
                normalJoin[2] = mPoints[i].zN;
                normalJoin[3] = 0;

                dataJoints += strideJoints;
            }

            dataSegments += strideSegments;
        }
    }

    void InstancingBasedPolylines::allocateVerticesBuffer()
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

    void InstancingBasedPolylines::allocateIndexesBuffer()
    {
        mIndicesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndices[0], sizeof(uint32_t) * mIndices.size()),
            BGFX_BUFFER_INDEX32);
    }
}