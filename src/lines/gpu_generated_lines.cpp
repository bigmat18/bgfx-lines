#include <lines/gpu_generated_lines.hpp>
#include <vclib/bgfx/context/load_program.h>

namespace lines {
    GPUGeneratedLines::GPUGeneratedLines(const std::vector<LinesVertex> &points) : 
        Lines("lines/cpu_generated_lines/vs_cpu_generated_lines", "lines/cpu_generated_lines/fs_cpu_generated_lines"),
        m_PointsSize(points.size())
    {
        m_ComputeProgram = bgfx::createProgram(vcl::loadShader("lines/gpu_generated_lines/cs_compute_buffers"), true);

        allocatePointsBuffer();
        allocateVertexBuffer();
        allocateIndexBuffer();

        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));
        generateBuffers();
    }

    GPUGeneratedLines::~GPUGeneratedLines() {
        if(bgfx::isValid(m_DIbh))
            bgfx::destroy(m_DIbh);

        if(bgfx::isValid(m_DVbh))
            bgfx::destroy(m_DVbh);

        if(bgfx::isValid(m_PointsBuffer))
            bgfx::destroy(m_PointsBuffer);
    }

    void GPUGeneratedLines::draw(uint viewId) const {
        m_Settings.bindUniformLines();

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

    void GPUGeneratedLines::update(const std::vector<LinesVertex> &points) {
        int oldSize = m_PointsSize;
        m_PointsSize = points.size();

        if(oldSize != m_PointsSize) {
            bgfx::destroy(m_DIbh);
            allocateIndexBuffer();
        }

        if(oldSize < m_PointsSize) {
            bgfx::destroy(m_DVbh);
            allocateVertexBuffer();
        } 

        if(oldSize > m_PointsSize) {
            bgfx::destroy(m_PointsBuffer);
            allocatePointsBuffer();
        }

        bgfx::update(m_PointsBuffer, 0, bgfx::makeRef(&points[0], sizeof(LinesVertex) * points.size()));
        generateBuffers();
    }

    void GPUGeneratedLines::generateBuffers() {
        bgfx::setBuffer(0, m_PointsBuffer, bgfx::Access::Read);
        bgfx::setBuffer(1, m_DVbh, bgfx::Access::Write);
        bgfx::setBuffer(2, m_DIbh, bgfx::Access::Write);
        bgfx::dispatch(0, m_ComputeProgram, (m_PointsSize / 2), 1, 1);
    }

    void GPUGeneratedLines::allocateVertexBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
         .end();

        m_DVbh = bgfx::createDynamicVertexBuffer(
            m_PointsSize * 4, layout, 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

    void GPUGeneratedLines::allocateIndexBuffer() {
        m_DIbh = bgfx::createDynamicIndexBuffer(
            m_PointsSize * 6, 
            BGFX_BUFFER_COMPUTE_WRITE | BGFX_BUFFER_ALLOW_RESIZE | BGFX_BUFFER_INDEX32
        );
    }

    void GPUGeneratedLines::allocatePointsBuffer() {
        bgfx::VertexLayout layout;
        layout
         .begin()
         .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
         .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8)
         .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
         .end();

        m_PointsBuffer = bgfx::createDynamicVertexBuffer(
            m_PointsSize, layout, 
            BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE
        );
    }

}