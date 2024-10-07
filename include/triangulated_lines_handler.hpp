#pragma once
#include <bgfx/bgfx.h>
#include <vector>

#include "lines.hpp"
#include "lines_handler.hpp"

namespace Lines
{
    class TriangulatedLinesHandler : public LinesHandler
    {
        friend LinesHandler *CreateHandler(LinesType type, const std::string name);
        friend void DestroyHandler(LinesHandler *handler);

    public:
        void EndLine() override;
        void Render(uint64_t state) override;

        void SetResolution(float width, float height)
        {
            m_data.resolution[0] = width;
            m_data.resolution[1] = height;
        }

        void SetColor(float r, float g, float b, float alpha)
        {
            m_data.color[0] = r;
            m_data.color[1] = g;
            m_data.color[2] = b;
            m_data.color[3] = alpha;
        }

        void SetAntialis(float antialias)
        {
            m_data.antialias = antialias;
        }

        void SetThickness(float thickness)
        {
            m_data.thickness = thickness;
        }

        ~TriangulatedLinesHandler();

    private:
        TriangulatedLinesHandler(uint64_t state, 
                                 const std::string vs_name, 
                                 const std::string fs_name, 
                                 const std::string name = "");

        bgfx::UniformHandle m_UniformData;
        bgfx::UniformHandle m_UniformColor;
        bgfx::UniformHandle m_UniformLength;

        typedef struct
        {
            float resolution[2];
            float antialias;
            float thickness;
            float color[4];
            float length;
        } LinesData;

        LinesData m_data;
    };

}