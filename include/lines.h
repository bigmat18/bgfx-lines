#pragma once
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <optional>
#include <vector>
#include "utils.h"

class Lines {
    public:
        typedef struct {
            float resolution[2];
            float antialias;
            float thickness;
            float color[4];
        } LinesData;

        typedef struct {
            float x;
            float y;
            float z;
        } Point;

    public:
        static void Init();
        static void Shutdown();        

        static void BeginLine(const float antialias, const float thickness);
        static void AddPoint(const Point& point);
        static void EndLine();

        static void SetResolution(float width, float height) {
            s_data.resolution[0] = width; 
            s_data.resolution[1] = height; 
        }

        static void SetColor(float r, float g, float b, float alpha) { 
            s_data.color[0] = r;
            s_data.color[1] = g;
            s_data.color[2] = b;
            s_data.color[3] = alpha;
        }

        static void RenderLine(uint64_t state);

    private:

        static float CalculateDistance(const Point& p1, const Point& p2);
 
        static void SetAntialis(float antialias) { s_data.antialias = antialias; }

        static void SetThickness(float thickness) { s_data.thickness = thickness; }
        
        static bgfx::ProgramHandle LoadProgram(const char* vs_name, const char* fs_name);

        static bgfx::ProgramHandle s_program;
        static bgfx::VertexBufferHandle s_vbh;
        static bgfx::IndexBufferHandle s_ibh;

        static const char* vs_name;
        static const char* fs_name;

        static bgfx::VertexLayout s_layout;
        static std::vector<float> s_vertices;
        static std::vector<uint32_t> s_indices;

        static bgfx::UniformHandle u_data;
        static bgfx::UniformHandle u_color;

        static bgfx::UniformHandle u_length;

        static LinesData s_data;

        static std::vector<Point> s_Points;
};
