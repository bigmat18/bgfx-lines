#ifndef HELLO_TRIANGLE_GLFW_H
#define HELLO_TRIANGLE_GLFW_H

#include <vclib/ext/glfw/canvas_window.h>
#include <primitive_drawable_lines.hpp>


class HelloTriangleGLFW : public vcl::glfw::CanvasWindow
{
public:
    HelloTriangleGLFW();

    ~HelloTriangleGLFW();

    void draw() override;

private:
    Lines::PrimitiveDrawableLines *primitive_line;
};

#endif // HELLO_TRIANGLE_GLFW_H
