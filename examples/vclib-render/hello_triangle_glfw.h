#pragma once

#include <vclib/glfw/canvas_window.h>
#include <primitive_drawable_lines.hpp>
#include <triangulated_drawable_lines.hpp>


class HelloTriangleGLFW : public vcl::glfw::CanvasWindow
{
public:
    HelloTriangleGLFW(uint32_t width, uint32_t heigth);

    ~HelloTriangleGLFW();

    void draw() override;

private:
    Lines::PrimitiveDrawableLines *primitive_line;
    Lines::TriangulatedDrawableLines *triangulated_line;
};
