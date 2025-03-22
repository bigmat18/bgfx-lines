#include <polylines/texture_based_polylines.hpp>

namespace lines
{

    const bgfx::ProgramHandle TextureBasedPolylines::mLinesPH = LoadProgram(
        "polylines/texture_based_polylines/vs_texture_based_segments",
        "polylines/texture_based_polylines/fs_texture_based_polylines");
        
    const bgfx::ProgramHandle TextureBasedPolylines::mJointsPH = LoadProgram(
        "polylines/texture_based_polylines/vs_texture_based_joins",
        "polylines/texture_based_polylines/fs_texture_based_polylines");
        
    const bgfx::ProgramHandle TextureBasedPolylines::mComputeTexturePH = bgfx::createProgram(
        LoadShader("polylines/texture_based_polylines/cs_compute_texture"), true);

    TextureBasedPolylines::TextureBasedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points),
        mJointsIndirectBH(bgfx::createIndirectBuffer(1)),
        mSegmentsIndirectBH(bgfx::createIndirectBuffer(1)),
        mComputeDataUH(bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
        allocateTextureBuffer();
        allocatePointsBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateTextureBuffer();
    }

    TextureBasedPolylines::TextureBasedPolylines(const TextureBasedPolylines &other) : 
        mPoints(other.mPoints),
        mJointsIndirectBH(bgfx::createIndirectBuffer(1)),
        mSegmentsIndirectBH(bgfx::createIndirectBuffer(1)),
        mComputeDataUH(bgfx::createUniform("u_IndirectData", bgfx::UniformType::Vec4))
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
        allocateTextureBuffer();
        allocatePointsBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateTextureBuffer();
    }

    TextureBasedPolylines::TextureBasedPolylines(TextureBasedPolylines &&other)
    {
        swap(other);
    }

    TextureBasedPolylines::~TextureBasedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndicesBH))
            bgfx::destroy(mIndicesBH);

        if (bgfx::isValid(mSegmentsIndirectBH))
            bgfx::destroy(mSegmentsIndirectBH);

        if (bgfx::isValid(mJointsIndirectBH))
            bgfx::destroy(mJointsIndirectBH);

        if (bgfx::isValid(mComputeDataUH))
            bgfx::destroy(mComputeDataUH);

        if (bgfx::isValid(mSegmentsTextureBH))
            bgfx::destroy(mSegmentsTextureBH);

        if (bgfx::isValid(mJointsTextureBH))
            bgfx::destroy(mJointsTextureBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);
    }

    TextureBasedPolylines &TextureBasedPolylines::operator=(TextureBasedPolylines other)
    {
        swap(other);
        return *this;
    }

    void TextureBasedPolylines::swap(TextureBasedPolylines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        std::swap(mPoints, other.mPoints);

        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndicesBH, other.mIndicesBH);
        std::swap(mPointsBH, other.mPointsBH);

        std::swap(mSegmentsIndirectBH, other.mSegmentsIndirectBH);
        std::swap(mJointsIndirectBH, other.mJointsIndirectBH);

        std::swap(mSegmentsTextureBH, other.mSegmentsTextureBH);
        std::swap(mJointsTextureBH, other.mJointsTextureBH);

        std::swap(mComputeDataUH, other.mComputeDataUH);
    }

    void TextureBasedPolylines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        float indirectData[] = {static_cast<float>(mPoints.size() - 1), static_cast<float>(mMaxTextureSize), 0, 0};
        bgfx::setUniform(mComputeDataUH, indirectData);

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndicesBH);
        bgfx::setImage(0, mSegmentsTextureBH, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH, mSegmentsIndirectBH, 0);

        if (bgfx::isValid(mJointsTextureBH) && settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndicesBH);
            bgfx::setImage(0, mJointsTextureBH, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mJointsPH, mJointsIndirectBH, 0);
        }
    }

    void TextureBasedPolylines::update(const std::vector<LinesVertex> &points)
    {
        int oldSize = mPoints.size();
        mPoints = points;

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));

        if (oldSize < mPoints.size())
        {
            allocateTextureBuffer();
        }

        generateTextureBuffer();
    }

    void TextureBasedPolylines::generateTextureBuffer()
    {
        float data[] = {static_cast<float>(mMaxTextureSize), static_cast<float>(mPoints.size() - 1), 0, 0};
        bgfx::setUniform(mComputeDataUH, data);

        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setImage(1, mSegmentsTextureBH, 0, bgfx::Access::Write);
        bgfx::setImage(2, mJointsTextureBH, 0, bgfx::Access::Write);
        bgfx::setBuffer(3, mSegmentsIndirectBH, bgfx::Access::Write);
        bgfx::setBuffer(4, mJointsIndirectBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeTexturePH, mPoints.size() - 1, 1, 1);
    }

    void TextureBasedPolylines::allocateTextureBuffer()
    {
        uint16_t Y_Segments = ((mPoints.size() - 1) * 5) / (mMaxTextureSize + 1);
        uint16_t X_Segments = Y_Segments == 0 ? ((mPoints.size() - 1) * 5) : mMaxTextureSize;

        mSegmentsTextureBH = bgfx::createTexture2D(
            X_Segments, Y_Segments + 1, false, 1, bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE);

        if (mPoints.size() > 2)
        {
            uint16_t Y_Joins = ((mPoints.size() - 2) * 4) / (mMaxTextureSize + 1);
            uint16_t X_Joins = Y_Joins == 0 ? ((mPoints.size() - 2) * 4) : mMaxTextureSize;

            mJointsTextureBH = bgfx::createTexture2D(
                X_Joins, Y_Joins + 1, false, 1, bgfx::TextureFormat::RGBA32F,
                BGFX_TEXTURE_COMPUTE_WRITE);
        }
    }

    void TextureBasedPolylines::allocatePointsBuffer()
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

    void TextureBasedPolylines::allocateVerticesBuffer()
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

    void TextureBasedPolylines::allocateIndexesBuffer()
    {
        mIndicesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndices[0], sizeof(uint32_t) * mIndices.size()),
            BGFX_BUFFER_INDEX32);
    }
}