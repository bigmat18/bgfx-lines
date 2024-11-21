#include <lines/gpu_generated_lines.hpp>
#include <vclib/render_bgfx/context/load_program.h>

namespace lines {
    GPUGeneratedLines::GPUGeneratedLines(const std::vector<Segment> &segments, const float width, const float heigth) : 
        Lines(width, heigth, "lines/cpu_generated_lines/vs_cpu_generated_lines", "lines/cpu_generated_lines/fs_cpu_generated_lines"),
        m_SegmentsSize(segments.size())
    {
        allocateSegmentsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        m_ComputeProgram = bgfx::createProgram(vcl::loadShader("lines/gpu_generated_lines/cs_compute_buffers"), true);
        bgfx::update(m_SegmentsBuffer, 0, bgfx::makeRef(&segments[0], sizeof(Segment) * segments.size()));

        generateBuffers();
    }

    GPUGeneratedLines::~GPUGeneratedLines() {
        bgfx::destroy(m_DIbh);
        bgfx::destroy(m_DVbh);
        bgfx::destroy(m_SegmentsBuffer);
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

    void GPUGeneratedLines::update(const std::vector<Segment> &segments) {
        int oldSize = m_SegmentsSize;
        m_SegmentsSize = segments.size();

        if(oldSize != m_SegmentsSize) {
            bgfx::destroy(m_DIbh);
            bgfx::destroy(m_DVbh);

            allocateIndexBuffer();
            allocateVertexBuffer();
        } 

        if(oldSize > m_SegmentsSize) {
            bgfx::destroy(m_SegmentsBuffer);
            allocateSegmentsBuffer();
        }

        bgfx::update(m_SegmentsBuffer, 0, bgfx::makeRef(&segments[0], sizeof(Segment) * segments.size()));
        generateBuffers();
    }

    void GPUGeneratedLines::generateBuffers() {
        bgfx::setBuffer(0, m_SegmentsBuffer, bgfx::Access::Read);
        bgfx::setBuffer(1, m_DVbh, bgfx::Access::Write);
        bgfx::setBuffer(2, m_DIbh, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeProgram, m_SegmentsSize, 1, 1);
    }

    void GPUGeneratedLines::allocateVertexBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 1, bgfx::AttribType::Float)
         .end();

        m_DVbh = bgfx::createDynamicVertexBuffer(
            m_SegmentsSize * 4, layout, 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void GPUGeneratedLines::allocateIndexBuffer() {
        m_DIbh = bgfx::createDynamicIndexBuffer(
            m_SegmentsSize * 6, 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );
    }

    void GPUGeneratedLines::allocateSegmentsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Float)
         .end();

        m_SegmentsBuffer = bgfx::createDynamicVertexBuffer(
            m_SegmentsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

}