#pragma once
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include "utils.h"

class Lines {
    public:
        static void Init();
        static void Shutdown();
        static void RenderLines(float x0, float y0, float x1, float y1);

    private:
        static bgfx::ProgramHandle LoadProgram(const char* vs_name, const char* fs_name);

        static bgfx::ProgramHandle s_program;
        static bgfx::VertexBufferHandle s_vbh;
        static bgfx::IndexBufferHandle s_ibh;

        static const char* vs_name;
        static const char* fs_name;
};