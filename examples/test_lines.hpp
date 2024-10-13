#pragma once
#include <vclib/ext/glfw/canvas_window.h>

class TestLines : public vcl::glfw::CanvasWindow {
    public:
        TestLines();

        ~TestLines();

        void draw() override;

    private:
        bgfx::VertexBufferHandle vbh;

        bgfx::ProgramHandle program;
};
