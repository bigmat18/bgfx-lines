#include <lines/gpu_generated_lines.hpp>

namespace lines
{
    bgfx::ProgramHandle GPUGeneratedLines::mLinesPH = LoadProgram(
        "lines/cpu_generated_lines/vs_cpu_generated_lines",
        "lines/cpu_generated_lines/fs_cpu_generated_lines");

    bgfx::ProgramHandle GPUGeneratedLines::mComputeVerticesPH = bgfx::createProgram(
        LoadShader("lines/gpu_generated_lines/cs_compute_buffers"), true);

    GPUGeneratedLines::GPUGeneratedLines(const std::vector<LinesVertex> &points) : mPoints(points)
    {
        allocateVertexBuffer();
        allocateIndexBuffer();
        allocatePointsBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    GPUGeneratedLines::GPUGeneratedLines(const GPUGeneratedLines &other) : mPointsBH(other.mPointsBH)
    {
        allocateIndexBuffer();
        allocateVertexBuffer();
        allocatePointsBuffer();

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    GPUGeneratedLines::GPUGeneratedLines(GPUGeneratedLines &&other)
    {
        swap(other);
    }

    GPUGeneratedLines::~GPUGeneratedLines()
    {
        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

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

        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mPointsBH, other.mPointsBH);

        std::swap(mPoints, other.mPoints);
    }

    void GPUGeneratedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }

    void GPUGeneratedLines::update(const std::vector<LinesVertex> &points)
    {
        int oldSize = mPoints.size();
        mPoints = points;

        if (oldSize != mPoints.size())
        {
            if (bgfx::isValid(mIndexesBH))
                bgfx::destroy(mIndexesBH);
            allocateIndexBuffer();
        }

        if (oldSize < mPoints.size())
        {
            if (bgfx::isValid(mVerticesBH))
                bgfx::destroy(mVerticesBH);
            allocateVertexBuffer();
        }

        if (oldSize > mPoints.size())
        {
            if (bgfx::isValid(mPointsBH))
                bgfx::destroy(mPointsBH);
            allocatePointsBuffer();
        }

        bgfx::update(mPointsBH, 0, bgfx::makeRef(&mPoints[0], sizeof(LinesVertex) * mPoints.size()));
        generateBuffers();
    }

    void GPUGeneratedLines::generateBuffers()
    {
        bgfx::setBuffer(0, mPointsBH, bgfx::Access::Read);
        bgfx::setBuffer(1, mVerticesBH, bgfx::Access::Write);
        bgfx::setBuffer(2, mIndexesBH, bgfx::Access::Write);
        bgfx::dispatch(0, mComputeVerticesPH, (mPoints.size() / 2), 1, 1);
    }

    void GPUGeneratedLines::allocateVertexBuffer()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
            .end();

        mVerticesBH = bgfx::createDynamicVertexBuffer(
            (mPoints.size() / 2) * 4, layout,
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE);
    }

    void GPUGeneratedLines::allocateIndexBuffer()
    {
        mIndexesBH = bgfx::createDynamicIndexBuffer(
            (mPoints.size() / 2) * 6,
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }

    void GPUGeneratedLines::allocatePointsBuffer()
    {
        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .end();

        mPointsBH = bgfx::createDynamicVertexBuffer(
            mPoints.size(), layout,
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
    }

}