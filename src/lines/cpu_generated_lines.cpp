#include <lines/cpu_generated_lines.hpp>

namespace lines
{
    CPUGeneratedLines::CPUGeneratedLines(const std::vector<LinesVertex> &points)
    {
        generateVerticesAndIndicesBuffers(points);
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
        using std::swap;

        GenericLines::swap(other);
        
        swap(mIndexesBH, other.mIndexesBH);
        swap(mVerticesBH, other.mVerticesBH);
        swap(mLinesPH, other.mLinesPH);
    }

    void CPUGeneratedLines::update(const std::vector<LinesVertex> &points)
    {
        generateVerticesAndIndicesBuffers(points);
    }

    void CPUGeneratedLines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mIndexesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);
    }

    void CPUGeneratedLines::generateVerticesAndIndicesBuffers(const std::vector<LinesVertex> &points)
    {
        uint32_t bufferVertsSize = points.size() * 4 * 12;
        uint32_t bufferIndsSize = (points.size() / 2) * 6;

        auto [vertices, vReleaseFn] =
            getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);

        auto [indices, iReleaseFn] =
            getAllocatedBufferAndReleaseFn<uint32_t>(bufferIndsSize);

        uint32_t vi = 0;
        uint32_t ii = 0;
        for (uint32_t i = 1; i < points.size(); i += 2)
        {
            for (uint32_t k = 0; k < 2; k++)
            {
                for (uint32_t j = 0; j < 2; j++)
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

            uint32_t index = (4 * (i / 2));
            indices[ii++] = index;
            indices[ii++] = index + 3;
            indices[ii++] = index + 1;

            indices[ii++] = index;
            indices[ii++] = index + 2;
            indices[ii++] = index + 3;
        }

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
            .end();

        mVerticesBH = bgfx::createVertexBuffer(
            bgfx::makeRef(vertices, sizeof(float) * bufferVertsSize, vReleaseFn),
            layout);

        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(indices, sizeof(uint32_t) * bufferIndsSize, iReleaseFn),
            BGFX_BUFFER_INDEX32);
    }
}