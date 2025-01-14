#include <polylines/cpu_generated_polylines.hpp>

namespace lines {
    CPUGeneratedPolylines::CPUGeneratedPolylines(const std::vector<LinesVertex> &points, const uint16_t width, const uint16_t heigth) :
        Polylines(width, heigth, "polylines/cpu_generated_polylines/vs_cpu_generated_polylines", "polylines/cpu_generated_polylines/fs_cpu_generated_polylines")
    {
        generateBuffers(points);
    }

    CPUGeneratedPolylines::~CPUGeneratedPolylines() {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_SegmentsIbh);
        bgfx::destroy(m_JoinsIbh);
    }

    void CPUGeneratedPolylines::draw(uint viewId) const {
        m_Settings.bindUniformPolylines();

        uint64_t state = 0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | UINT64_C(0)
            | BGFX_STATE_BLEND_ALPHA;

        bgfx::setVertexBuffer(0, m_Vbh);
        bgfx::setIndexBuffer(m_SegmentsIbh);
        bgfx::setState(state);
        bgfx::submit(viewId, m_Program);

        if(m_Settings.getJoin() != 0) {
            bgfx::setVertexBuffer(0, m_Vbh);
            bgfx::setIndexBuffer(m_JoinsIbh);
            bgfx::setState(state);
            bgfx::submit(viewId, m_Program);
        }
    }

    void CPUGeneratedPolylines::update(const std::vector<LinesVertex> &points) {
        bgfx::destroy(m_Vbh);
        bgfx::destroy(m_SegmentsIbh);
        bgfx::destroy(m_JoinsIbh);

        m_Vertices.clear();
        m_SegmentsIndices.clear();
        m_JoinsIndices.clear();

        generateBuffers(points);
    }

    void CPUGeneratedPolylines::generateBuffers(const std::vector<LinesVertex> points) {

        for(uint i = 0; i < points.size() - 1; i++) {

            for(uint k = 0; k < 2; k++) {
                
                for(uint j = 0; j < 2; j++) {
                    uint curr_index = i + k;
                    uint prev_index = curr_index - (curr_index == 0 ? 0 : 1);
                    uint next_index = curr_index + (curr_index == points.size() - 1 ? 0 : 1);

                    m_Vertices.push_back(points[prev_index].X);
                    m_Vertices.push_back(points[prev_index].Y);
                    m_Vertices.push_back(points[prev_index].Z);

                    m_Vertices.push_back(points[curr_index].X);
                    m_Vertices.push_back(points[curr_index].Y);
                    m_Vertices.push_back(points[curr_index].Z);

                    m_Vertices.push_back(points[next_index].X);
                    m_Vertices.push_back(points[next_index].Y);
                    m_Vertices.push_back(points[next_index].Z);

                    m_Vertices.push_back(points[curr_index].getReverseColor());

                    m_Vertices.push_back(points[curr_index].xN);
                    m_Vertices.push_back(points[curr_index].yN);
                    m_Vertices.push_back(points[curr_index].zN);

                    m_Vertices.push_back(static_cast<float>(k));
                    m_Vertices.push_back(static_cast<float>(j));
                }
            }

            m_SegmentsIndices.push_back((i * 4));
            m_SegmentsIndices.push_back((i * 4) + 3);
            m_SegmentsIndices.push_back((i * 4) + 1);

            m_SegmentsIndices.push_back((i * 4));
            m_SegmentsIndices.push_back((i * 4) + 2);
            m_SegmentsIndices.push_back((i * 4) + 3);

            if(i != points.size() - 2) {
                m_JoinsIndices.push_back((i * 4) + 3);
                m_JoinsIndices.push_back((i * 4) + 4);
                m_JoinsIndices.push_back((i * 4) + 5);

                m_JoinsIndices.push_back((i * 4) + 4);
                m_JoinsIndices.push_back((i * 4) + 2);
                m_JoinsIndices.push_back((i * 4) + 5);
            }
        }

        bgfx::VertexLayout layout;
        layout
            .begin()
            .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord2, 2, bgfx::AttribType::Float)
            .end();

        m_Vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(&m_Vertices[0], sizeof(float) * m_Vertices.size()),
            layout
        );

        m_SegmentsIbh = bgfx::createIndexBuffer(
            bgfx::makeRef(&m_SegmentsIndices[0], sizeof(uint32_t) * m_SegmentsIndices.size()),
            BGFX_BUFFER_INDEX32
        );

        if(m_JoinsIndices.size() != 0)
            m_JoinsIbh = bgfx::createIndexBuffer(
                bgfx::makeRef(&m_JoinsIndices[0], sizeof(uint32_t) * m_JoinsIndices.size()),
                BGFX_BUFFER_INDEX32
            );
    }
}