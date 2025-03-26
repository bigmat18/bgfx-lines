#include <polylines/cpu_generated_polylines.hpp>

namespace lines
{
    const bgfx::ProgramHandle CPUGeneratedPolylines::mLinesPH = LoadProgram(
        "polylines/cpu_generated_polylines/vs_cpu_generated_polylines",
        "polylines/cpu_generated_polylines/fs_cpu_generated_polylines");

    CPUGeneratedPolylines::CPUGeneratedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        allocateVertexBuffer();
        allocateIndexesBuffer();
        generateBuffers();
    }

    CPUGeneratedPolylines::CPUGeneratedPolylines(const CPUGeneratedPolylines &other) :
        mPoints(other.mPoints)
    {
        allocateVertexBuffer();
        allocateIndexesBuffer();
        generateBuffers();
    }

    CPUGeneratedPolylines::CPUGeneratedPolylines(CPUGeneratedPolylines &&other)
    {
        swap(other);
    }

    CPUGeneratedPolylines::~CPUGeneratedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mSegmentsIndexesBH))
            bgfx::destroy(mSegmentsIndexesBH);

        if (bgfx::isValid(mJoinsIndexesBH))
            bgfx::destroy(mJoinsIndexesBH);
    }

    CPUGeneratedPolylines &CPUGeneratedPolylines::operator=(CPUGeneratedPolylines other)
    {
        swap(other);
        return *this;
    }

    void CPUGeneratedPolylines::swap(CPUGeneratedPolylines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        std::swap(mPoints, other.mPoints);
        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mSegmentsIndexesBH, other.mSegmentsIndexesBH);
        std::swap(mJoinsIndexesBH, other.mJoinsIndexesBH);
    }

    void CPUGeneratedPolylines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mSegmentsIndexesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (bgfx::isValid(mJoinsIndexesBH) && settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mJoinsIndexesBH);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mLinesPH);
        }
    }

    void CPUGeneratedPolylines::update(const std::vector<LinesVertex> &points)
    {
        if (mPoints.size() > points.size())
        {
            bgfx::destroy(mVerticesBH);
            bgfx::destroy(mSegmentsIndexesBH);
            bgfx::destroy(mJoinsIndexesBH);

            allocateVertexBuffer();
            allocateIndexesBuffer();
        }

        mPoints = points;
        generateBuffers();
    }

    void CPUGeneratedPolylines::generateBuffers()
    {
        if (mPoints.size() > 1) {
            uint32_t bufferVertsSize = (mPoints.size() - 1) * 4 * 15;
            uint32_t bufferSegmetIndicesSize = (mPoints.size() - 1) * 6;
            uint32_t bufferJointsIndicesSize = (mPoints.size() - 2) * 6;
    
            auto [vertices, vReleaseFn] =
                getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);
    
            auto [segmIndices, siReleaseFn] =
                getAllocatedBufferAndReleaseFn<uint32_t>(bufferSegmetIndicesSize);
    
            auto [jointIndices, jiReleaseFn] =
                getAllocatedBufferAndReleaseFn<uint32_t>(bufferJointsIndicesSize);
    
            uint32_t vi = 0;
            uint32_t si = 0;
            uint32_t ji = 0;
    
            for (uint32_t i = 0; i < mPoints.size() - 1; i++) {
                for (uint32_t k = 0; k < 2; k++) {
                    for (uint32_t j = 0; j < 2; j++) {
                        uint32_t curr_index = i + k;
                        uint32_t prev_index = curr_index - (curr_index == 0 ? 0 : 1);
                        uint32_t next_index =
                            curr_index + (curr_index == mPoints.size() - 1 ? 0 : 1);
    
                        vertices[vi++] = mPoints[prev_index].X;
                        vertices[vi++] = mPoints[prev_index].Y;
                        vertices[vi++] = mPoints[prev_index].Z;
    
                        vertices[vi++] = mPoints[curr_index].X;
                        vertices[vi++] = mPoints[curr_index].Y;
                        vertices[vi++] = mPoints[curr_index].Z;
    
                        vertices[vi++] = mPoints[next_index].X;
                        vertices[vi++] = mPoints[next_index].Y;
                        vertices[vi++] = mPoints[next_index].Z;
    
                        vertices[vi++] = std::bit_cast<float>(
                            mPoints[curr_index].getABGRColor());
    
                        vertices[vi++] = mPoints[curr_index].xN;
                        vertices[vi++] = mPoints[curr_index].yN;
                        vertices[vi++] = mPoints[curr_index].zN;
    
                        vertices[vi++] = static_cast<float>(k);
                        vertices[vi++] = static_cast<float>(j);
                    }
                }
    
                segmIndices[si++] = (i * 4);
                segmIndices[si++] = (i * 4) + 3;
                segmIndices[si++] = (i * 4) + 1;
    
                segmIndices[si++] = (i * 4);
                segmIndices[si++] = (i * 4) + 2;
                segmIndices[si++] = (i * 4) + 3;
    
                if (mPoints.size() > 2 && i != mPoints.size() - 2) {
                    jointIndices[ji++] = (i * 4) + 3;
                    jointIndices[ji++] = (i * 4) + 4;
                    jointIndices[ji++] = (i * 4) + 5;
    
                    jointIndices[ji++] = (i * 4) + 4;
                    jointIndices[ji++] = (i * 4) + 2;
                    jointIndices[ji++] = (i * 4) + 5;
                }
            }
            bgfx::update(mVerticesBH, 0, bgfx::makeRef(&vertices[0], sizeof(float) * bufferVertsSize));
            bgfx::update(mSegmentsIndexesBH, 0, bgfx::makeRef(&segmIndices[0], sizeof(uint32_t) * bufferSegmetIndicesSize));
            bgfx::update(mJoinsIndexesBH, 0, bgfx::makeRef(&jointIndices[0], sizeof(uint32_t) * bufferJointsIndicesSize));
        }
    }

    void CPUGeneratedPolylines::allocateVertexBuffer()
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

        mVerticesBH = bgfx::createDynamicVertexBuffer(
            (mPoints.size() - 1) * 4, layout,
            BGFX_BUFFER_ALLOW_RESIZE);
    }

    void CPUGeneratedPolylines::allocateIndexesBuffer()
    {
        mSegmentsIndexesBH = bgfx::createDynamicIndexBuffer(
            (mPoints.size() - 1) * 6,
            BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);

        mJoinsIndexesBH = bgfx::createDynamicIndexBuffer(
            (mPoints.size() - 2) * 6,
            BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }
}