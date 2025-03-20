#include <lines/cpu_generated_lines.hpp>

namespace lines
{
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex> &points) : mPointsSize(points.size())
    {
        allocateVertexBuffer();
        allocateIndexBuffer();
        generateBuffers(points);
    }

    CPUGeneratedLines::CPUGeneratedLines(const CPUGeneratedLines &other) : mPointsSize(other.mPointsSize),
                                                                           mIndexes(other.mIndexes),
                                                                           mVertices(other.mVertices)
    {
        allocateIndexBuffer();
        allocateVertexBuffer();
        generateBuffers(points);
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

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }

    CPUGeneratedLines &CPUGeneratedLines::operator=(CPUGeneratedLines other)
    {
        swap(other);
        return *this;
    }

    void CPUGeneratedLines::swap(CPUGeneratedLines &other)
    {
        std::swap(mSettings, other.mSettings);
        std::swap(mPointsSize, other.mPointsSize);
        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mVerticesBH, other.mVerticesBH);
    }

    void CPUGeneratedLines::update(const std::vector<LinesVertex> &points)
    {
        if (mPointsSize > points.size())
        {
            bgfx::destroy(mVerticesBH);
            bgfx::destroy(mIndexesBH);

            allocateVertexBuffer();
            allocateIndexBuffer();
        }

        mPointsSize = points.size();
        generateBuffers(points);
    }

    void CPUGeneratedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }

    void CPUGeneratedLines::generateBuffers(const std::vector<LinesVertex> &points)
    {
        uint bufferVertsSize = points.size() * 4 * 12;
        uint bufferIndsSize = (points.size() / 2) * 6;

        auto [vertices, vReleaseFn] =
            getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

        auto [indices, iReleaseFn] =
            getAllocatedBufferAndReleaseFn<uint>(bufferIndsSize);

        uint vi = 0;
        uint ii = 0;
        for (uint i = 1; i < points.size(); i += 2)
        {
            for (uint k = 0; k < 2; k++)
            {
                for (uint j = 0; j < 2; j++)
                {
                    vertices[vi++] = points[i - 1].X;
                    vertices[vi++] = points[i - 1].Y;
                    vertices[vi++] = points[i - 1].Z;

                    vertices[vi++] = points[i].X;
                    vertices[vi++] = points[i].Y;
                    vertices[vi++] = points[i].Z;

                    vertices[vi++] = std::bit_cast<float>(
                        points[i - (1 - k)].getABGRColor());

                    vertices[vi++] = points[i - (1 - k)].xN;
                    vertices[vi++] = points[i - (1 - k)].yN;
                    vertices[vi++] = points[i - (1 - k)].zN;

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
            (mPointsSize / 2) * 4, layout,
            BGFX_BUFFER_ALLOW_RESIZE);
    }

    void CPUGeneratedLines::allocateIndexBuffer()
    {
        mIndexesBH = bgfx::createDynamicIndexBuffer(
            (mPointsSize / 2) * 6,
            BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32);
    }
}