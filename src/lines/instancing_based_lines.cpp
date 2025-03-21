#include <lines/instancing_based_lines.hpp>

namespace lines
{
    bgfx::ProgramHandle InstancingBasedLines::mLinesPH = LoadProgram(
        "lines/instancing_based_lines/vs_instancing_based_lines",
        "lines/instancing_based_lines/fs_instancing_based_lines");

    InstancingBasedLines::InstancingBasedLines(const std::vector<LinesVertex> &points) : mPoints(points)
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
    }

    InstancingBasedLines::InstancingBasedLines(const InstancingBasedLines &other) : mPoints(other.mPoints)
    {
        allocateVerticesBuffer();
        allocateIndexesBuffer();
    }

    InstancingBasedLines::InstancingBasedLines(InstancingBasedLines &&other)
    {
        swap(other);
    }

    InstancingBasedLines::~InstancingBasedLines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);
    }

    InstancingBasedLines &InstancingBasedLines::operator=(InstancingBasedLines other)
    {
        swap(other);
        return *this;
    }

    void InstancingBasedLines::swap(InstancingBasedLines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        std::swap(mPoints, other.mPoints);

        std::swap(mVerticesBH, other.mVerticesBH);
        std::swap(mIndexesBH, other.mIndexesBH);
        std::swap(mInstanceDB, other.mInstanceDB);
    }

    void InstancingBasedLines::draw(uint32_t viewId) const
    {
        if (mPoints.size() > 1)
        {
            generateInstanceDataBuffer();
            bindSettingsUniform();

            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mIndexesBH);
            bgfx::setInstanceDataBuffer(&mInstanceDB);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mLinesPH);
        }
    }

    void InstancingBasedLines::update(const std::vector<LinesVertex> &points)
    {
        mPoints = points;
        generateInstanceDataBuffer();
    }

    void InstancingBasedLines::generateInstanceDataBuffer() const
    {
        const uint16_t stride = sizeof(float) * 16;

        uint32_t linesNum = bgfx::getAvailInstanceDataBuffer((mPoints.size() / 2), stride);
        bgfx::allocInstanceDataBuffer(&mInstanceDB, linesNum, stride);

        uint8_t *data = mInstanceDB.data;
        for (uint32_t i = 1; i < linesNum * 2; i += 2)
        {

            float *p0 = reinterpret_cast<float *>(data);
            p0[0] = mPoints[i - 1].X;
            p0[1] = mPoints[i - 1].Y;
            p0[2] = mPoints[i - 1].Z;

            uint32_t *color0 = (uint32_t *)&data[12];
            color0[0] = mPoints[i - 1].getABGRColor();

            float *p1 = (float *)&data[16];
            p1[0] = mPoints[i].X;
            p1[1] = mPoints[i].Y;
            p1[2] = mPoints[i].Z;

            uint32_t *color1 = (uint32_t *)&data[28];
            color1[0] = mPoints[i].getABGRColor();

            float *n0 = (float *)&data[32];
            n0[0] = mPoints[i - 1].xN;
            n0[1] = mPoints[i - 1].yN;
            n0[2] = mPoints[i - 1].zN;
            n0[3] = 0;

            float *n1 = (float *)&data[48];
            n1[0] = mPoints[i].xN;
            n1[1] = mPoints[i].yN;
            n1[2] = mPoints[i].zN;
            n1[3] = 0;

            data += stride;
        }
    }

    void InstancingBasedLines::allocateVerticesBuffer()
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

    void InstancingBasedLines::allocateIndexesBuffer()
    {
        mIndexesBH = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndexes[0], sizeof(uint32_t) * mIndexes.size()),
            BGFX_BUFFER_INDEX32);
    }
}