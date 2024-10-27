#include <triangulated_drawable_lines.hpp>

namespace Lines {

    TriangulatedDrawableLines::TriangulatedDrawableLines(const std::string vs_name, const std::string fs_name, const std::string name) : 
        DrawableLines(vs_name, fs_name, name)
    {
        mLayout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        mUniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
        mUniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
        mUniformLength = bgfx::createUniform("u_length", bgfx::UniformType::Vec4);

    }

    TriangulatedDrawableLines::~TriangulatedDrawableLines() {
        bgfx::destroy(mUniformData);
        bgfx::destroy(mUniformColor);
        bgfx::destroy(mUniformLength);
    }

    void TriangulatedDrawableLines::endLine() {
        if(isClosed())
            mPoints.push_back(mPoints[0]);

        std::vector<std::array<float, 4>> prev(mPoints.size());
        std::vector<std::array<float, 4>> curr(mPoints.size());
        std::vector<std::array<float, 4>> next(mPoints.size());

        for(uint32_t i = 0; i < mPoints.size(); i++) {
            mData.length += calculateDistance(mPoints[i-1 != -1 ? i-1 : 0], mPoints[i]);
            std::array<float, 4> element = {mPoints[i].x, mPoints[i].y, mPoints[i].z, mData.length};

            curr[i] = element;

            if (i == 0) prev[i] = element;
            else prev[i] = {mPoints[i - 1].x, mPoints[i - 1].y, mPoints[i - 1].z, 0.0f};

            if (i == mPoints.size() - 1) next[i] = element;
            else next[i] = {mPoints[i+1].x, mPoints[i+1].y, mPoints[i+1].z, 0.0f};
        }

        if(isClosed()) {
            prev[0] = curr[mPoints.size() - 2];
            next[mPoints.size() - 1] = curr[1];
        }

        uint32_t i = 0;
        for(; i < curr.size() * 2; i++) {
        
            // a_position ==> prev(x,y,z)
            mVertices.push_back(prev[i/2][0]);
            mVertices.push_back(prev[i/2][1]);
            mVertices.push_back(prev[i/2][2]);

            // a_texcoord0 ==> curr(x,y,z)
            mVertices.push_back(curr[i/2][0]);
            mVertices.push_back(curr[i/2][1]);
            mVertices.push_back(curr[i/2][2]);

            // a_texcoord1 ==> next(x,y,z)
            mVertices.push_back(next[i/2][0]);
            mVertices.push_back(next[i/2][1]);
            mVertices.push_back(next[i/2][2]);

            // a_texcoord2 ==> uv(up/low, distance)
            mVertices.push_back(i % 2 == 0 ? 1.0 : -1.0);
            mVertices.push_back(curr[i/2][3]);
        }

        uint32_t j = 0;
        for(uint32_t i = 0; i < mPoints.size() - 1; i++, j+=2) {
            mIndices.push_back(j);
            mIndices.push_back(j+1);
            mIndices.push_back(j+2);

            mIndices.push_back(j+1);
            mIndices.push_back(j+3);
            mIndices.push_back(j+2);
        }

        mVbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&mVertices[0], sizeof(float) * mVertices.size()),
            mLayout
        );

        mIbh = bgfx::createIndexBuffer(
            bgfx::makeRef(&mIndices[0], sizeof(uint32_t) * mIndices.size()),
            BGFX_BUFFER_INDEX32
        );
    }

    void TriangulatedDrawableLines::draw(uint viewId) const {
        float data[] = {mData.resolution[0], mData.resolution[1], mData.antialias, mData.thickness};
        bgfx::setUniform(mUniformData, data);
        bgfx::setUniform(mUniformColor, mData.color);

        float dataLength[] = {mData.length, 0.0, 0.0, 0.0};
        bgfx::setUniform(mUniformLength, dataLength);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, mVbh);
        bgfx::setIndexBuffer(mIbh);
        bgfx::setState(state);
        bgfx::submit(viewId, mProgram);
    }
}
