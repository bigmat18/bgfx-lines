#include <lines/gpu_generated_lines.hpp>

namespace lines
{

    GPUGeneratedLines::GPUGeneratedLines(const std::vector<LinesVertex> &points) :
        mPointsSize(points.size())
    {
        checkCaps();    
        allocateAndSetPointsBuffer(points);
        allocateVertexBuffer();
        allocateIndexBuffer();
        generateVerticesAndIndicesBuffers();
    }

    GPUGeneratedLines::GPUGeneratedLines(GPUGeneratedLines &&other)
    {
        swap(other);
    }

    GPUGeneratedLines::~GPUGeneratedLines()
    {
        if (bgfx::isValid(mIndicesBH))
            bgfx::destroy(mIndicesBH);

        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mPointsBH))
            bgfx::destroy(mPointsBH);
    }

    GPUGeneratedLines &GPUGeneratedLines::operator=(GPUGeneratedLines other)
    {
        swap(other);
        return *this;
    }

    void GPUGeneratedLines::swap(GPUGeneratedLines &other)
    {
        using std::swap;

        GenericLines::swap(other);

        swap(mIndicesBH, other.mIndicesBH);
        swap(mVerticesBH, other.mVerticesBH);
        swap(mPointsBH, other.mPointsBH);
    }

    void GPUGeneratedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndicesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }

    void GPUGeneratedLines::update(const std::vector<LinesVertex> &points)
    {
        uint32_t oldSize = mPointsSize;
        mPointsSize = points.size();

        if (oldSize != mPointsSize)
        {
            if (bgfx::isValid(mIndicesBH))
                bgfx::destroy(mIndicesBH);
            allocateIndexBuffer();
        }

        if (oldSize < mPointsSize)
        {
            if (bgfx::isValid(mVerticesBH))
                bgfx::destroy(mVerticesBH);
            allocateVertexBuffer();
        }

        if (oldSize > mPointsSize)
        {
            if (bgfx::isValid(mPointsBH))
                bgfx::destroy(mPointsBH);
            allocateAndSetPointsBuffer(points);
        }

        generateVerticesAndIndicesBuffers();
    }

    void GPUGeneratedLines::allocateAndSetPointsBuffer(
        const std::vector<LinesVertex>& points)
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();
    
        auto [buffer, releaseFn] =
            getAllocatedBufferAndReleaseFn<LinesVertex>(points.size());
    
        std::copy(points.begin(), points.end(), buffer);
    
        mPointsBH = bgfx::createVertexBuffer(
            bgfx::makeRef(buffer, sizeof(LinesVertex) * points.size(), releaseFn),
            layout,
            BGFX_BUFFER_COMPUTE_READ);
    }
    
    void GPUGeneratedLines::allocateVertexBuffer()
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
            .end();
    
    
        const uint32_t sz = (mPointsSize / 2) * 4 * 12;
    
        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(nullptr, sizeof(float) * sz),
            layout,
            BGFX_BUFFER_COMPUTE_READ_WRITE);
    }
    
    void GPUGeneratedLines::allocateIndexBuffer()
    {
        const uint32_t sz = (mPointsSize / 2) * 6;
    
        mIndicesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(nullptr, sizeof(uint32_t) * sz),
            BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
    }
    
    void GPUGeneratedLines::generateVerticesAndIndicesBuffers()
    {
        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setBuffer(1, mVerticesBH, bgfx::Access::Write);
        bgfx::setBuffer(2, mIndicesBH, bgfx::Access::Write);

        bgfx::dispatch(0, mComputeVerticesPH, (mPointsSize / 2), 1, 1);
    }

}