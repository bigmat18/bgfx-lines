#include <lines/texture_based_lines.hpp>
#include <assert.h>

namespace lines
{
    TextureBasedLines::TextureBasedLines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        allocateVerticesAndIndicesBuffers();
        allocateAndSetPointsBuffer();
        allocateAndGenerateTextureBuffer();
    }

    TextureBasedLines::~TextureBasedLines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if (bgfx::isValid(mIndirectBH))
            bgfx::destroy(mIndirectBH);

        if (bgfx::isValid(mTextureBH))
            bgfx::destroy(mTextureBH);

        if (bgfx::isValid(mIndirectDataUH))
            bgfx::destroy(mIndirectDataUH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);

        if (bgfx::isValid(mComputeTexturePH))
            bgfx::destroy(mComputeTexturePH);
    }

    void TextureBasedLines::swap(TextureBasedLines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        swap(mPoints, other.mPoints);

        swap(mTextureBH, other.mTextureBH);
        swap(mPointsBH, other.mPointsBH);

        swap(mIndirectBH, other.mIndirectBH);
        swap(mIndirectDataUH, other.mIndirectDataUH);

        swap(mVerticesBH, other.mVerticesBH);
        swap(mIndexesBH, other.mIndexesBH);

        swap(mLinesPH, other.mLinesPH);
        swap(mComputeTexturePH, other.mComputeTexturePH);
    }

    void TextureBasedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setImage(0, mTextureBH, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA32F);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
    }

    void TextureBasedLines::update(const std::vector<LinesVertex> &points)
    {
        mPoints = points;
        allocateAndSetPointsBuffer();
        allocateAndGenerateTextureBuffer();
    }

    void TextureBasedLines::allocateAndGenerateTextureBuffer()
    {
        uint16_t Y = (mPoints.size() * 3) / (mMaxTextureSize + 1);
        uint16_t X = Y == 0 ? (mPoints.size() * 3) : mMaxTextureSize;

        mTextureBH = bgfx::createTexture2D(
            X, Y + 1, false, 1, bgfx::TextureFormat::RGBA32F,
            BGFX_TEXTURE_COMPUTE_WRITE);

        float data[] = {static_cast<float>(mMaxTextureSize), static_cast<float>(mPoints.size() / 2), 0, 0};
        bgfx::setUniform(mIndirectDataUH, data);

        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setImage(1, mTextureBH, 0, bgfx::Access::Write);
        bgfx::setBuffer(2, mIndirectBH, bgfx::Access::ReadWrite);
        bgfx::dispatch(0, mComputeTexturePH, (mPoints.size() / 2), 1, 1);
    }

    void TextureBasedLines::allocateAndSetPointsBuffer()
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
            layout, BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
    }

    void TextureBasedLines::allocateVerticesAndIndicesBuffers()
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