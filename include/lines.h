#pragma once
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include "utils.h"

class Lines {
    public:
        typedef struct {
            float resolution[2];
            float antialias;
            float thickness;
            float color[4];
        } LinesData;

    public:
        static void Init();
        static void Shutdown();
        static void RenderLines(float x0, float y0, float x1, float y1);

        static void SetResolution(float width, float height) {
            s_data.resolution[0] = width; 
            s_data.resolution[1] = height; 
        }
        
        static void SetAntialis(float antialis) { s_data.antialias = antialis; }

        static void SetThickness(float thickness) { s_data.thickness = thickness; }

        static void SetColor(float r, float g, float b, float alpha) { 
            s_data.color[0] = r;
            s_data.color[1] = g;
            s_data.color[2] = b;
            s_data.color[3] = alpha;
        }

    private:
        static bgfx::ProgramHandle LoadProgram(const char* vs_name, const char* fs_name);

        static bgfx::ProgramHandle s_program;
        static bgfx::VertexBufferHandle s_vbh;
        static bgfx::IndexBufferHandle s_ibh;

        static const char* vs_name;
        static const char* fs_name;

        static bgfx::VertexLayout s_layout;
        static const float s_vertices[];
        static const uint32_t s_indices[];

        static bgfx::UniformHandle u_resolution;
        static bgfx::UniformHandle u_antialias;
        static bgfx::UniformHandle u_thickness;
        static bgfx::UniformHandle u_color;

        static bgfx::UniformHandle u_p0;
        static bgfx::UniformHandle u_p1;

        static LinesData s_data;
};