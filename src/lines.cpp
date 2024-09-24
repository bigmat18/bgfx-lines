#include "lines.h"
#include "bgfx/bgfx.h"
#include <array>
#include <bgfx_utils.h>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

Lines::LinesData            Lines::s_data;
std::vector<Lines::Point>   Lines::s_Points; 

bgfx::ProgramHandle         Lines::s_program;
bgfx::VertexBufferHandle    Lines::s_vbh;
bgfx::IndexBufferHandle     Lines::s_ibh;

const char*                 Lines::vs_name = "vs_lines";
const char*                 Lines::fs_name = "fs_lines";

bgfx::UniformHandle         Lines::u_data;
bgfx::UniformHandle         Lines::u_color;
bgfx::UniformHandle         Lines::u_length;

bgfx::VertexLayout          Lines::s_layout;

std::vector<float>          Lines::s_vertices;
std::vector<uint32_t>       Lines::s_indices;


void Lines::Init() {
    s_layout
        .begin()
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, false)
        .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Float, false)
        .add(bgfx::Attrib::Color2, 4, bgfx::AttribType::Float, false)
        .end();

    s_program = Lines::LoadProgram(vs_name, fs_name);

    u_data = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
    u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    u_length = bgfx::createUniform("u_length", bgfx::UniformType::Vec4);
}

void Lines::Shutdown() {
    bgfx::destroy(s_ibh);
    bgfx::destroy(s_vbh),
    bgfx::destroy(s_program);
    bgfx::destroy(u_data);
    bgfx::destroy(u_color);
    bgfx::destroy(u_length);
}

void Lines::BeginLine(const float antialias, const float thickness) {
    s_Points.clear();

    Lines::SetAntialis(antialias);
    Lines::SetThickness(thickness);

    std::cout << s_data.antialias << s_data.thickness << std::endl;
}

void Lines::AddPoint(const Lines::Point& point) {
    Lines::s_Points.push_back(point);
}

void Lines::EndLine() {
    std::vector<std::array<std::array<float, 4>, 2>> prev(s_Points.size());
    std::vector<std::array<std::array<float, 4>, 2>> curr(s_Points.size());
    std::vector<std::array<std::array<float, 4>, 2>> next(s_Points.size());

    float length = 0;

    for(uint32_t i = 0; i < s_Points.size(); i++) {
        length += CalculateDistance(s_Points[i-1 != -1 ? i-1 : 0], s_Points[i]);

        std::array<std::array<float, 4>, 2> element = {{
            {s_Points[i].x, s_Points[i].y, 1.0, length},
            {s_Points[i].x, s_Points[i].y, -1.0, length}
        }};

        curr[i] = element;

        if (i == 0) prev[i] = element;
        else {
            prev[i] = {{
                {s_Points[i-1].x, s_Points[i-1].y, 1.0f, curr[i-1][0][3]}, 
                {s_Points[i-1].x, s_Points[i-1].y, -1.0f, curr[i-1][0][3]}, 
            }};
        }

        if (i == s_Points.size() - 1) next[i] = element;
        else {
            next[i] = {{
                {s_Points[i+1].x, s_Points[i+1].y, 1.0f, curr[i][0][3] + CalculateDistance(s_Points[i], s_Points[i+1])}, 
                {s_Points[i+1].x, s_Points[i+1].y, -1.0f, curr[i][0][3] + CalculateDistance(s_Points[i], s_Points[i+1])}, 
            }};
        }
    }
    float dataLength[] = {length, 0.0, 0.0, 0.0};
    bgfx::setUniform(u_length,dataLength);

    for(uint32_t i = 0; i < curr.size(); i++) {
        std::cout << "prev0: " << prev[i][0][0] << ", " << prev[i][0][1] << ", " << prev[i][0][2] << ", "<<prev[i][0][3] << std::endl;
        std::cout << "prev1: " << prev[i][1][0] << ", " << prev[i][1][1] << ", " << prev[i][1][2] << ", "<<prev[i][1][3] << std::endl << std::endl;


        std::cout << "curr0: " << curr[i][0][0] << ", " << curr[i][0][1] << ", " << curr[i][0][2] << ", "<<curr[i][0][3] << std::endl;
        std::cout << "curr1: " << curr[i][1][0] << ", " << curr[i][1][1] << ", " << curr[i][1][2] << ", "<<curr[i][1][3] << std::endl << std::endl;

        std::cout << "next0: " << next[i][0][0] << ", " << next[i][0][1] << ", " << next[i][0][2] << ", "<<next[i][0][3] << std::endl;
        std::cout << "next1: " << next[i][1][0] << ", " << next[i][1][1] << ", " << next[i][1][2] << ", "<<next[i][1][3] << std::endl << std::endl;
    
        s_vertices.push_back(prev[i][0][0]);
        s_vertices.push_back(prev[i][0][1]);
        s_vertices.push_back(prev[i][0][2]);
        s_vertices.push_back(prev[i][0][3]);


        s_vertices.push_back(curr[i][0][0]);
        s_vertices.push_back(curr[i][0][1]);
        s_vertices.push_back(curr[i][0][2]);
        s_vertices.push_back(curr[i][0][3]);

        s_vertices.push_back(next[i][0][0]);
        s_vertices.push_back(next[i][0][1]);
        s_vertices.push_back(next[i][0][2]);
        s_vertices.push_back(next[i][0][3]);



        s_vertices.push_back(prev[i][0][0]);
        s_vertices.push_back(prev[i][0][1]);
        s_vertices.push_back(prev[i][0][2]);
        s_vertices.push_back(prev[i][0][3]);

        s_vertices.push_back(curr[i][1][0]);
        s_vertices.push_back(curr[i][1][1]);
        s_vertices.push_back(curr[i][1][2]);
        s_vertices.push_back(curr[i][1][3]);

        s_vertices.push_back(next[i][0][0]);
        s_vertices.push_back(next[i][0][1]);
        s_vertices.push_back(next[i][0][2]);
        s_vertices.push_back(next[i][0][3]);
    }

    s_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(s_vertices.data(), sizeof(float) * s_vertices.size()),
        s_layout
    );

    for(uint32_t i = 0; i < s_Points.size(); i+=2) {
        s_indices.push_back(i);
        s_indices.push_back(i+1);
        s_indices.push_back(i+2);

        s_indices.push_back(i+1);
        s_indices.push_back(i+3);
        s_indices.push_back(i+2);
    }

    s_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_indices.data(), sizeof(uint32_t) * s_indices.size())
    );

    for(int i = 0; i < s_vertices.size(); i+=12) {
        std::cout << s_vertices[i] << " " << s_vertices[i+1] << " " << s_vertices[i+2] << " " << s_vertices[i+3] << std::endl;
        std::cout << s_vertices[i+4] << " " << s_vertices[i+5] << " " << s_vertices[i+6] << " " << s_vertices[i+7] << std::endl;
        std::cout << s_vertices[i+8] << " " << s_vertices[i+9] << " " << s_vertices[i+10] << " " << s_vertices[i+11] << std::endl << std::endl;
    }
        
    for(int i = 0; i < s_indices.size(); i+=3)
        std::cout << s_indices[i] << " " << s_indices[i+1] << " " << s_indices[i+2] << " " << std::endl;

    //std::cout << s_data.antialias << " " << s_data.thickness << std::endl;
    //std::cout << s_data.color[0] << " " << s_data.color[1] << " " << s_data.color[2] << " " << s_data.color[3] << std::endl;
}

void Lines::RenderLine(uint64_t state) {
    float data[] = {s_data.resolution[0], s_data.resolution[1], s_data.antialias, s_data.thickness};
    bgfx::setUniform(u_data, data);
    bgfx::setUniform(u_color, s_data.color);
 
    bgfx::setVertexBuffer(0, s_vbh);
    bgfx::setIndexBuffer(s_ibh);
    bgfx::setState(state);
    bgfx::submit(0, s_program);
}

bgfx::ProgramHandle Lines::LoadProgram(const char *vs_name, const char *fs_name) {
    return loadProgram(vs_name, fs_name);
}

float Lines::CalculateDistance(const Lines::Point& p1, const Lines::Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}
