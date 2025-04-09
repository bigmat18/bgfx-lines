#include <lines/instancing_based_lines.hpp>

namespace lines
{
    InstancingBasedLines::InstancingBasedLines(const std::vector<LinesVertex> &points) : mPoints(points)
    {
        checkCaps();
        allocateVerticesAndIndicesBuffers();
    }

    InstancingBasedLines::~InstancingBasedLines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mIndexesBH))
            bgfx::destroy(mIndexesBH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }

    void InstancingBasedLines::swap(InstancingBasedLines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        swap(mPoints, other.mPoints);

        swap(mVerticesBH, other.mVerticesBH);
        swap(mIndexesBH, other.mIndexesBH);
        swap(mInstanceDB, other.mInstanceDB);

        swap(mLinesPH, other.mLinesPH);
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
            float* p0 = reinterpret_cast<float*>(data);
            p0[0]     = mPoints[i - 1].X;
            p0[1]     = mPoints[i - 1].Y;
            p0[2]     = mPoints[i - 1].Z;
    
            uint32_t* color0 = reinterpret_cast<uint32_t*>(&data[12]);
            color0[0]    = mPoints[i - 1].getRGBAColor();
    
            float* p1 = reinterpret_cast<float*>(&data[16]);
            p1[0]     = mPoints[i].X;
            p1[1]     = mPoints[i].Y;
            p1[2]     = mPoints[i].Z;
    
            uint32_t* color1 = reinterpret_cast<uint32_t*>(&data[28]);
            color1[0]    = mPoints[i].getRGBAColor();
    
            float* n0 = reinterpret_cast<float*>(&data[32]);
            n0[0]     = mPoints[i - 1].xN;
            n0[1]     = mPoints[i - 1].yN;
            n0[2]     = mPoints[i - 1].zN;
            n0[3]     = 0;
    
            float* n1 = reinterpret_cast<float*>(&data[48]);
            n1[0]     = mPoints[i].xN;
            n1[1]     = mPoints[i].yN;
            n1[2]     = mPoints[i].zN;
            n1[3]     = 0;
    
            data += stride;
        }
    }

    void InstancingBasedLines::allocateVerticesAndIndicesBuffers()
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