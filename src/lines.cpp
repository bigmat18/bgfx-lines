#include "lines.h"
#include <bgfx_utils.h>

Lines::LinesData Lines::s_data;

bgfx::ProgramHandle         Lines::s_program;
bgfx::VertexBufferHandle    Lines::s_vbh;
bgfx::IndexBufferHandle     Lines::s_ibh;

const char*                 Lines::vs_name = "vs_lines";
const char*                 Lines::fs_name = "fs_lines";

bgfx::UniformHandle         Lines::u_resolution;
bgfx::UniformHandle         Lines::u_antialias;
bgfx::UniformHandle         Lines::u_thickness;
bgfx::UniformHandle         Lines::u_p0;
bgfx::UniformHandle         Lines::u_p1;
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

    u_resolution = bgfx::createUniform("u_resolution", bgfx::UniformType::Vec4);
    u_antialias = bgfx::createUniform("u_antialias", bgfx::UniformType::Vec4);
    u_thickness = bgfx::createUniform("u_thickness", bgfx::UniformType::Vec4);

    u_p0 = bgfx::createUniform("u_p0", bgfx::UniformType::Vec4);
    u_p1 = bgfx::createUniform("u_p1", bgfx::UniformType::Vec4);
}

void Lines::Shutdown() {
    bgfx::destroy(s_ibh);
    bgfx::destroy(s_vbh),
    bgfx::destroy(s_program);
}

void Lines::RenderLines(float x0, float y0, float x1, float y1) {
    float resolution[] = {s_data.resolution[0], s_data.resolution[1], 0.0f, 0.0f};
    bgfx::setUniform(u_resolution, resolution);

    float antialis[] = {s_data.antialias, 0.0f, 0.0f, 0.0f};
    bgfx::setUniform(u_antialias, antialis);

    float thickness[] = {s_data.thickness, 0.0f, 0.0f, 0.0f};
    bgfx::setUniform(u_thickness, thickness);

    float p0[] = {x0, y0, 0.0f, 0.0f};
    bgfx::setUniform(u_p0, p0);

    float p1[] = {x1, y1, 0.0f, 0.0f};
    bgfx::setUniform(u_p1, p1);

    bgfx::setVertexBuffer(0, s_vbh);
    bgfx::setIndexBuffer(s_ibh);
    bgfx::submit(0, s_program);
}

bgfx::ProgramHandle Lines::LoadProgram(const char *vs_name, const char *fs_name) {
    return loadProgram(vs_name, fs_name);
}