#include <polylines/cpu_generated_polylines.hpp>

namespace lines
{
    CPUGeneratedPolylines::CPUGeneratedPolylines(const std::vector<LinesVertex> &points)
    {
        generateVerticesAndIndicesBuffers(points);
    }

    CPUGeneratedPolylines::~CPUGeneratedPolylines()
    {
        if (bgfx::isValid(mVerticesBH))
            bgfx::destroy(mVerticesBH);

        if (bgfx::isValid(mSegmentIndicesBH))
            bgfx::destroy(mSegmentIndicesBH);

        if (bgfx::isValid(mJointIndicesBH))
            bgfx::destroy(mJointIndicesBH);

        if (bgfx::isValid(mLinesPH))
            bgfx::destroy(mLinesPH);
    }

    void CPUGeneratedPolylines::swap(CPUGeneratedPolylines &other)
    {
        using std::swap;
        GenericLines::swap(other);

        swap(mVerticesBH, other.mVerticesBH);
        swap(mSegmentIndicesBH, other.mSegmentIndicesBH);
        swap(mJointIndicesBH, other.mJointIndicesBH);
    }

    void CPUGeneratedPolylines::draw(uint32_t viewId) const
    {
        bindSettingsUniform();

        bgfx::setVertexBuffer(0, mVerticesBH);
        bgfx::setIndexBuffer(mSegmentIndicesBH);
        bgfx::setState(drawState());
        bgfx::submit(viewId, mLinesPH);

        if (bgfx::isValid(mJointIndicesBH) && settings().getJoint() != PolyLineJoint::ROUND_JOINT)
        {
            bgfx::setVertexBuffer(0, mVerticesBH);
            bgfx::setIndexBuffer(mJointIndicesBH);
            bgfx::setState(drawState());
            bgfx::submit(viewId, mLinesPH);
        }
    }

    void CPUGeneratedPolylines::update(const std::vector<LinesVertex> &points)
    {
        generateVerticesAndIndicesBuffers(points);
    }

    void CPUGeneratedPolylines::generateVerticesAndIndicesBuffers(const std::vector<LinesVertex>& points)
    {
        if (points.size() > 1) {
            uint32_t bufferVertsSize = (points.size() - 1) * 4 * 16;
            uint32_t bufferSegmetIndicesSize = (points.size() - 1) * 6;
            uint32_t bufferJointsIndicesSize = (points.size() - 2) * 6;
    
            auto [vertices, vReleaseFn] =
                getAllocatedBufferAndReleaseFn<float>(bufferVertsSize);
    
            auto [segmIndices, siReleaseFn] =
                getAllocatedBufferAndReleaseFn<uint32_t>(bufferSegmetIndicesSize);
    
            auto [jointIndices, jiReleaseFn] =
                getAllocatedBufferAndReleaseFn<uint32_t>(bufferJointsIndicesSize);
    
            uint32_t vi = 0;
            uint32_t si = 0;
            uint32_t ji = 0;
    
            for (uint32_t i = 0; i < points.size() - 1; i++) {
                for (uint32_t k = 0; k < 2; k++) {
                    for (uint32_t j = 0; j < 2; j++) {
                        uint32_t curr_index = i + k;
                        uint32_t prev_index = curr_index - (curr_index == 0 ? 0 : 1);
                        uint32_t next_index =
                            curr_index + (curr_index == points.size() - 1 ? 0 : 1);
    
                        vertices[vi++] = points[prev_index].X;
                        vertices[vi++] = points[prev_index].Y;
                        vertices[vi++] = points[prev_index].Z;
    
                        vertices[vi++] = points[curr_index].X;
                        vertices[vi++] = points[curr_index].Y;
                        vertices[vi++] = points[curr_index].Z;
    
                        vertices[vi++] = points[next_index].X;
                        vertices[vi++] = points[next_index].Y;
                        vertices[vi++] = points[next_index].Z;
    
                        vertices[vi++] = std::bit_cast<float>(
                            points[curr_index].getABGRColor());
    
                        vertices[vi++] = points[curr_index].xN;
                        vertices[vi++] = points[curr_index].yN;
                        vertices[vi++] = points[curr_index].zN;
    
                        vertices[vi++] = static_cast<float>(k);
                        vertices[vi++] = static_cast<float>(j);
                        vertices[vi++] = 0;
                    }
                }
    
                segmIndices[si++] = (i * 4);
                segmIndices[si++] = (i * 4) + 3;
                segmIndices[si++] = (i * 4) + 1;
    
                segmIndices[si++] = (i * 4);
                segmIndices[si++] = (i * 4) + 2;
                segmIndices[si++] = (i * 4) + 3;
    
                if (points.size() > 2 && i != points.size() - 2) {
                    jointIndices[ji++] = (i * 4) + 3;
                    jointIndices[ji++] = (i * 4) + 4;
                    jointIndices[ji++] = (i * 4) + 5;
    
                    jointIndices[ji++] = (i * 4) + 4;
                    jointIndices[ji++] = (i * 4) + 2;
                    jointIndices[ji++] = (i * 4) + 5;
                }
            }
            bgfx::VertexLayout layout;
            layout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord2, 3, bgfx::AttribType::Float)
                .end();
    
            mVerticesBH = bgfx::createVertexBuffer(
                bgfx::makeRef(vertices, sizeof(float) * bufferVertsSize, vReleaseFn),
                layout
            );
    
            mSegmentIndicesBH = bgfx::createIndexBuffer(
                bgfx::makeRef(segmIndices, sizeof(uint32_t) * bufferSegmetIndicesSize, siReleaseFn),
                BGFX_BUFFER_INDEX32
            );
    
            if (points.size() > 2) {
                mJointIndicesBH = bgfx::createIndexBuffer(
                    bgfx::makeRef(jointIndices, sizeof(uint32_t) * bufferJointsIndicesSize, jiReleaseFn),
                    BGFX_BUFFER_INDEX32
                );
            }
        }
    }
}