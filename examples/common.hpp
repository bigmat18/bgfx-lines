#ifndef COMMON_H
#define COMMON_H

#include <bgfx/bgfx.h>

#include <vclib/space/core/color.h>
#include <vclib_bgfx/render/context/load_program.h>

struct Vertex
{
    float pos[2];
    uint32_t abgr;
};

static const Vertex vertices[]{
    {{-1.0f, -1.0f}, vcl::Color(vcl::Color::Red).abgr()},
    {{1.0f, -1.0f}, vcl::Color(vcl::Color::Green).abgr()},
    {{0.0f, 1.0f}, vcl::Color(vcl::Color::Blue).abgr()},
};

inline void setUpBGFX(
    bgfx::ViewId viewId,
    bgfx::VertexBufferHandle &vbh,
    bgfx::ProgramHandle &program)
{
    vcl::Color backgroundColor = vcl::Color::Black;

    bgfx::setViewClear(
        viewId,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        backgroundColor.rgba(),
        1.0f,
        0);

    bgfx::VertexLayout layout;

    layout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();

    vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices, sizeof(vertices)), layout);

    program = vcl::loadProgram(
        "vs_primitive_shader", "fs_primitive_shader");

    bgfx::touch(viewId);
}

inline void drawOnView(
    bgfx::ViewId viewId,
    const bgfx::VertexBufferHandle &vbh,
    const bgfx::ProgramHandle &program)
{
    bgfx::setVertexBuffer(0, vbh);

    bgfx::submit(viewId, program);
}

#endif // COMMON_H
