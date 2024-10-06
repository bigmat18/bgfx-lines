#pragma once
#include "lines_handler.hpp"

namespace Lines {

    class PrimitiveLinesHandler : public LinesHandler {
        friend class LinesFactory;

        public:
            void EndLine() override;
            void Render(uint64_t state) override;

            void SetColor(float r, float g, float b, float alpha)
            {
                m_color[0] = r;
                m_color[1] = g;
                m_color[2] = b;
                m_color[3] = alpha;
            }

        private:
            PrimitiveLinesHandler(uint64_t state, const std::string vs_name, const std::string fs_name, const std::string name = "");
            ~PrimitiveLinesHandler() = default;

            float m_color[4];
            bgfx::UniformHandle m_UniformColor;
    };
}