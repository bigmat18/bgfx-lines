#include <lines/indirect_based_lines.hpp>

namespace lines
{

    IndirectBasedLines::IndirectBasedLines(const std::vector<LinesVertex> &points) :
        mPoints(points)
    {
        checkCaps();
        allocateVerticesAndIndicesBuffers();
        generateIndirectBuffer();
        allocateAndSetPointsBuffer();
    }

    IndirectBasedLines::~IndirectBasedLines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);

        if (bgfx::isValid(mIndirectBH))
            bgfx::destroy(mIndirectBH);

        if (bgfx::isValid(mIndirectDataUH))
            bgfx::destroy(mIndirectDataUH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);

        if (bgfx::isValid(mComputeIndirectPH))
            bgfx::destroy(mComputeIndirectPH);
    }

    void IndirectBasedLines::swap(IndirectBasedLines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        swap(mVerticesBH, other.mVerticesBH);
        swap(mIndexesBH, other.mIndexesBH);
        swap(mPointsBH, other.mPointsBH);
        swap(mIndirectBH, other.mIndirectBH);
        swap(mIndirectDataUH, other.mIndirectDataUH);

        swap(mLinesPH, other.mLinesPH);
        swap(mComputeIndirectPH, other.mComputeIndirectPH);
    }

    void IndirectBasedLines::allocateAndSetPointsBuffer()
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

    void IndirectBasedLines::allocateVerticesAndIndicesBuffers()
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

    void IndirectBasedLines::generateIndirectBuffer()
    {
        float data[] = {static_cast<float>(mPoints.size() / 2), 0, 0, 0};
        bgfx::setUniform(mIndirectDataUH, data);
        bgfx::setBuffer(0, mIndirectBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeIndirectPH);
    }

    void IndirectBasedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();
        
        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);

        bgfx::setBuffer(1, mPointsBH, bgfx::Access::Read);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH, mIndirectBH, 0);
    }

    void IndirectBasedLines::update(const std::vector<LinesVertex> &points)
    {
        int oldSize = mPoints.size();
        mPoints = points;

        if (oldSize != mPoints.size())
            generateIndirectBuffer();

        allocateAndSetPointsBuffer();
    }
}