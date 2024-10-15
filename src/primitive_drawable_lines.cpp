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
        if(bgfx::isValid(mProgram)) {
            bgfx::setUniform(mUniformColor, mColor);

            bgfx::setVertexBuffer(0, mVbh);
            bgfx::setIndexBuffer(mIbh);
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                           BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                           BGFX_STATE_PT_LINES |
                           BGFX_STATE_BLEND_FUNC(
                               BGFX_STATE_BLEND_SRC_ALPHA,
                               BGFX_STATE_BLEND_INV_SRC_ALPHA));
            bgfx::submit(viewId, mProgram);

        }
    }
}