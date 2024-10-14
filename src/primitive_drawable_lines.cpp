#include <primitive_drawable_lines.hpp>
#include <iostream>

namespace Lines {
    PrimitiveDrawableLines::PrimitiveDrawableLines(const std::string vs_name, const std::string fs_name, const std::string name) : 
        DrawableLines(vs_name, fs_name, name)
    {
        mLayout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();

        mUniformColor = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    }

    void PrimitiveDrawableLines::endLine() {
        for(uint32_t i = 0; i < mPoints.size(); i++) {
            mVertices.push_back(mPoints[i].x);
            mVertices.push_back(mPoints[i].y);
            mVertices.push_back(mPoints[i].x);
        }

        for(uint32_t i = 0; i < mPoints.size() - 1; i++) {
            mIndices.push_back(i);
            mIndices.push_back(i + 1);
        }

        if(isClosed()) {
            mIndices.push_back(0);
            mIndices.push_back(mPoints.size() - 1);
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

    void PrimitiveDrawableLines::draw(uint viewId) const {
        bgfx::setUniform(mUniformColor, mColor);

        uint64_t state = 0
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_ALWAYS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | BGFX_STATE_PT_LINES
            | BGFX_STATE_BLEND_ALPHA;
        
        bgfx::setVertexBuffer(viewId, mVbh);
        bgfx::setIndexBuffer(mIbh);
        bgfx::setState(state);
        bgfx::submit(viewId, mProgram);
    }
}