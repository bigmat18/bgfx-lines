#include "lines.h"
#include <array>
#include <bgfx_utils.h>
#include <cmath>
#include <cstdint>
#include <vector>

Lines::LinesData Lines::s_data;
std::vector<Lines::Point> Lines::s_Points; 

bgfx::ProgramHandle         Lines::s_program;
bgfx::VertexBufferHandle    Lines::s_vbh;
bgfx::IndexBufferHandle     Lines::s_ibh;

const char*                 Lines::vs_name = "vs_lines";
const char*                 Lines::fs_name = "fs_lines";

bgfx::UniformHandle         Lines::u_data;
bgfx::UniformHandle         Lines::u_color;
bgfx::UniformHandle         Lines::u_prev;
bgfx::UniformHandle         Lines::u_curr;
bgfx::UniformHandle         Lines::u_next;

bgfx::VertexLayout          Lines::s_layout;

const float                 Lines::s_vertices[] = {
                                0.0f, 1.0f, 0.0f, // A1
                                0.0f, 0.0f, 0.0f, // A2
                                1.0f, 1.0f, 0.0f, // B1
                                1.0f, 0.0f, 0.0f, // B2
                            };

const uint32_t              Lines::s_indices[] = {
                                0, 1, 2,
                                1, 3, 2,
                            };


void Lines::Init() {
    s_layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .end();

    s_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_vertices, sizeof(s_vertices)),
        s_layout
    );
    s_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_indices, sizeof(s_indices))
    );
    s_program = Lines::LoadProgram(vs_name, fs_name);

    u_data = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
    u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);

    u_prev = bgfx::createUniform("u_prev", bgfx::UniformType::Vec4);
    u_curr = bgfx::createUniform("u_curr", bgfx::UniformType::Vec4);
    u_next = bgfx::createUniform("u_next", bgfx::UniformType::Vec4);
}

void Lines::Shutdown() {
    bgfx::destroy(s_ibh);
    bgfx::destroy(s_vbh),
    bgfx::destroy(s_program);
    bgfx::destroy(u_data);
    bgfx::destroy(u_color);
    bgfx::destroy(u_prev);
    bgfx::destroy(u_curr);
    bgfx::destroy(u_next);
}

void Lines::BeginLine(const std::optional<float>& width,
                      const std::optional<float>& height,
                      const std::optional<float>& antialias,
                      const std::optional<float>& thickness) {
    s_Points.clear();

    if(width && height) Lines::SetResolution(width.value(), height.value());
    if(antialias) Lines::SetAntialis(antialias.value());
    if(thickness) Lines::SetThickness(thickness.value());
}

void Lines::AddPoint(const Lines::Point& point) {
    Lines::s_Points.push_back(point);
}

void Lines::EndLine(uint64_t state) {
    float data[] = {s_data.resolution[0], s_data.resolution[1], s_data.antialias, s_data.thickness};
    bgfx::setUniform(u_data, data);
    bgfx::setUniform(u_color, s_data.color);

    std::vector<std::array<std::array<float, 4>, 2>> prev(s_Points.size());
    std::vector<std::array<std::array<float, 4>, 2>> curr(s_Points.size());
    std::vector<std::array<std::array<float, 4>, 2>> next(s_Points.size());

    float length = 0;

    for(uint32_t i = 0; i < s_Points.size(); i++) {
        std::array<std::array<float, 4>, 2> element = {{
            {s_Points[i].x, s_Points[i].y, 1.0, length + CalculateDistance(s_Points[i-1 == -1 ? i : 0], s_Points[i])},
            {s_Points[i].x, s_Points[i].y, -1.0, length + CalculateDistance(s_Points[i-1 == -1 ? i : 0], s_Points[i])}
        }};

        curr[i] = element;

        if (i == 0) {
            prev[i] = element;
        } else {
            prev[i] = {{
                {s_Points[i-1].x, s_Points[i-1].y, 1.0f, CalculateDistance(s_Points[i-2 == -1 ? i : 0], s_Points[i-1])}, 
                {s_Points[i-1].x, s_Points[i-1].y, -1.0f, CalculateDistance(s_Points[i-2 == -1 ? i : 0], s_Points[i-1])}, 
            }};
        }

        if (i == s_Points.size() - 1) {
            next[i] = element;
        } else {
            next[i] = prev[i] = {{
                {s_Points[i+1].x, s_Points[i+1].y, 1.0f, CalculateDistance(s_Points[i], s_Points[i+1])}, 
                {s_Points[i+1].x, s_Points[i+1].y, -1.0f, CalculateDistance(s_Points[i], s_Points[i+1])}, 
            }};
        }
    }

    bgfx::setVertexBuffer(0, s_vbh);
    bgfx::setIndexBuffer(s_ibh);
    bgfx::setState(state);

    bgfx::submit(0, s_program);   
}

bgfx::ProgramHandle Lines::LoadProgram(const char *vs_name, const char *fs_name) {
    return loadProgram(vs_name, fs_name);
}

float CalculateDistance(const Lines::Point& p1, const Lines::Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}
