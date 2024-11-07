#include <gpu_generated_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>

namespace lines {
    GPUGeneratedLines::GPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth) : 
        Lines(width, heigth, "cpu_generated_lines/vs_cpu_generated_lines", "cpu_generated_lines/fs_cpu_generated_lines")
    {
        // bgfx::VertexLayout layoutDVbh;
        // layoutDVbh
        //  .begin()
        //  .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        //  .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        //  .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        //  .end();

        // m_DVbh = bgfx::createDynamicVertexBuffer((points.size() / 2) * 4, layoutDVbh, BGFX_BUFFER_COMPUTE_WRITE);
        // m_DIbh = bgfx::createDynamicIndexBuffer((points.size() / 2) * 6, BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_INDEX32);

        // bgfx::VertexLayout layoutPointsBuffer;
        // layoutPointsBuffer
        //  .begin()
        //  .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        //  .end();

        // m_PointsBuffer = bgfx::createDynamicVertexBuffer(
        //     bgfx::makeRef(&points[0], sizeof(Point) * points.size()),
        //     layoutPointsBuffer,
        //     BGFX_BUFFER_COMPUTE_READ
        // );
        // m_ComputeProgram = bgfx::createProgram(vcl::loadShader("gpu_generated_lines/cs_compute_buffers.sc"), true);

        // bgfx::setBuffer(0, m_PointsBuffer, bgfx::Access::Read);
        // bgfx::setBuffer(1, m_DVbh, bgfx::Access::Write);
        // bgfx::setBuffer(2, m_DIbh, bgfx::Access::Write);
        // bgfx::dispatch(0, m_ComputeProgram, points.size() / 2, 1, 1);
    }

    void GPUGeneratedLines::draw(uint viewId) const {
        float data[] = {m_Data.screenSize[0], m_Data.screenSize[1], m_Data.antialias, m_Data.thickness};
        bgfx::setUniform(m_UniformData, data);

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_DVbh);
        bgfx::setIndexBuffer(m_DIbh);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);
    }
}