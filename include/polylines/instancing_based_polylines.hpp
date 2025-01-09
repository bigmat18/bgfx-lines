#pragma once
#include "../polylines.hpp"

namespace lines {
    class InstancingBasedPolylines : public Polylines {
        public:
            InstancingBasedPolylines(const std::vector<Point> &points, const uint16_t width, const uint16_t heigth);

            ~InstancingBasedPolylines();

            std::shared_ptr<vcl::DrawableObjectI> clone() const override {
                return std::make_shared<InstancingBasedPolylines>(*this);
            }

            void draw(uint viewId) const override;

            void update(const std::vector<Point> &points) override;

        private:
            void generateIDBSegments(const std::vector<Point> &points);

            void generateIDBJoins(const std::vector<Point> &points);

            bgfx::InstanceDataBuffer m_IDBSegments;
            bgfx::InstanceDataBuffer m_IDBJoins;

            std::vector<float> m_Vertices;
            std::vector<uint32_t> m_Indices;

            bgfx::VertexBufferHandle m_Vbh;
            bgfx::IndexBufferHandle m_Ibh;

            bgfx::ProgramHandle m_JoinsProgram;

    };
}