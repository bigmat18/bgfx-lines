#include <lines/cpu_generated_lines.hpp>

namespace lines
{
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex> &points) : 
        mPoints(points)
    {
        mLinesPH = LoadProgram(
            "lines/cpu_generated_lines/vs_cpu_generated_lines", 
            "lines/cpu_generated_lines/fs_cpu_generated_lines");
        allocateVertexBuffer();
        allocateIndexBuffer();
        generateBuffers();
    }

    CPUGeneratedLines::CPUGeneratedLines(const CPUGeneratedLines &other) : 
        mPoints(other.mPoints)
    {
        allocateIndexBuffer();
        allocateVertexBuffer();
        generateBuffers();
    }

    CPUGeneratedLines::CPUGeneratedLines(CPUGeneratedLines &&other)
    {
        swap(other);
    }

    CPUGeneratedLines::~CPUGeneratedLines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);
    }

    CPUGeneratedLines &CPUGeneratedLines::operator=(CPUGeneratedLines other)
    {
        swap(other);
        return *this;
    }

    void CPUGeneratedLines::swap(CPUGeneratedLines &other)
    {
        using std::swap;

        GenericLines::swap(other);
        
        swap(mPoints, other.mPoints);
        swap(mIndexesBH, other.mIndexesBH);
        swap(mVerticesBH, other.mVerticesBH);
    }

    void CPUGeneratedLines::update(const std::vector<LinesVertex> &points)
    {
        if (mPoints.size() > points.size())
        {
            bgfx::destroy(mVerticesBH);
            bgfx::destroy(mIndexesBH);

            allocateVertexBuffer();
            allocateIndexBuffer();
        }

        mPoints = points;
        generateBuffers();
    }

    void CPUGeneratedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }

    void CPUGeneratedLines::generateBuffers()
    {
        uint bufferVertsSize = mPoints.size() * 4 * 12;
        uint bufferIndsSize = (mPoints.size() / 2) * 6;

        auto [vertices, vReleaseFn] =
            getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

        auto [indices, iReleaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(bufferIndsSize);

        uint vi = 0;
        uint ii = 0;
        for (uint i = 1; i < mPoints.size(); i += 2)
        {
            for (uint k = 0; k < 2; k++)
            {
                for (uint j = 0; j < 2; j++)
                {
                    vertices[vi++] = mPoints[i - 1].X;
                    vertices[vi++] = mPoints[i - 1].Y;
                    vertices[vi++] = mPoints[i - 1].Z;

                    vertices[vi++] = mPoints[i].X;
                    vertices[vi++] = mPoints[i].Y;
                    vertices[vi++] = mPoints[i].Z;

                    vertices[vi++] = std::bit_cast<float>(
                        mPoints[i - (1 - k)].getABGRColor());

                    vertices[vi++] = mPoints[i - (1 - k)].xN;
                    vertices[vi++] = mPoints[i - (1 - k)].yN;
                    vertices[vi++] = mPoints[i - (1 - k)].zN;

                    vertices[vi++] = k;
                    vertices[vi++] = j;
                }
            }

            uint index = (4 * (i / 2));
            indices[ii++] = index;
            indices[ii++] = index + 3;
            indices[ii++] = index + 1;

            indices[ii++] = index;
            indices[ii++] = index + 2;
            indices[ii++] = index + 3;
        }

        bgfx::update(mVerticesBH, 0, bgfx::makeRef(&vertices, sizeof(float) * bufferVertsSize));
        bgfx::update(mIndexesBH, 0, bgfx::makeRef(&indices, sizeof(uint32_t) * bufferIndsSize));
    }

    void CPUGeneratedLines::allocateVertexBuffer()
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
            BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_COMPUTE_READ);
    }

    void CPUGeneratedLines::allocateIndexBuffer()
    {
        mIndexesBH = bgfx::createDynamicIndexBuffer(
            (mPoints.size() / 2) * 6,
            BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32 | BGFX_BUFFER_COMPUTE_READ);
    }
}