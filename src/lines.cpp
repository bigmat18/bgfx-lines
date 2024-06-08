#include "lines.h"
#include <bgfx_utils.h>

bgfx::ProgramHandle         Lines::s_program;
bgfx::VertexBufferHandle    Lines::s_vbh;
bgfx::IndexBufferHandle     Lines::s_ibh;

const char*                 Lines::vs_name = "vs_lines";
const char*                 Lines::fs_name = "fs_lines";

static bgfx::VertexLayout s_layout;

static float s_vertices[] = 
{
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
};

static uint32_t s_indices[] = 
{
    0, 1, 2,
    1, 2, 3
};

void Lines::Init() {
    s_program = Lines::LoadProgram(vs_name, fs_name);
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
}

void Lines::Shutdown() {
    bgfx::destroy(s_ibh);
    bgfx::destroy(s_vbh),
    bgfx::destroy(s_program);
}

void Lines::RenderLines(float x0, float y0, float x1, float y1) {
    bgfx::setVertexBuffer(0, s_vbh);
    bgfx::setIndexBuffer(s_ibh);
    bgfx::submit(0, s_program);
}

bgfx::ProgramHandle Lines::LoadProgram(const char *vs_name, const char *fs_name) {
    return loadProgram(vs_name, fs_name);
}