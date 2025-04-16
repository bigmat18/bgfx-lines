#include <polylines/texture_based_polylines.hpp>

namespace lines
{

    TextureBasedPolylines::TextureBasedPolylines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        checkCaps();

        allocateVerticesAndIndicesBuffers();
        allocateAndSetPointsBuffer();
        allocateTextureBuffers();
        generateTextureBuffers();
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

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);

        if (bgfx::isValid(mComputeTexturePH))
            bgfx::destroy(mComputeTexturePH);
    }

    void TextureBasedPolylines::swap(TextureBasedPolylines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        swap(mPoints, other.mPoints);

        swap(mVerticesBH, other.mVerticesBH);
        swap(mIndicesBH, other.mIndicesBH);
        swap(mPointsBH, other.mPointsBH);

        swap(mSegmentsIndirectBH, other.mSegmentsIndirectBH);
        swap(mJointsIndirectBH, other.mJointsIndirectBH);

        swap(mSegmentsTextureBH, other.mSegmentsTextureBH);
        swap(mJointsTextureBH, other.mJointsTextureBH);

        swap(mComputeDataUH, other.mComputeDataUH);
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

        allocateAndSetPointsBuffer();

        if (oldSize < mPoints.size())
        {
            allocateTextureBuffers();
        }

        generateTextureBuffers();
    }

    void TextureBasedPolylines::generateTextureBuffers()
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

    void TextureBasedPolylines::allocateTextureBuffers()
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

    void TextureBasedPolylines::allocateAndSetPointsBuffer()
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

    void TextureBasedPolylines::allocateVerticesAndIndicesBuffers()
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