#include "hello_triangle_glfw.h"
#include <primitive_drawable_lines.hpp>
#include <triangulated_drawable_lines.hpp>

HelloTriangleGLFW::HelloTriangleGLFW(uint32_t width, uint32_t heigth) :
        vcl::glfw::CanvasWindow("Hello Triangle GLFW", width, heigth)
{
    vcl::Color backgroundColor = vcl::Color::Black;

    bgfx::setViewClear(
        viewId(),
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        backgroundColor.rgba(),
        1.0f,
        0);

    // primitive_line = dynamic_cast<Lines::PrimitiveDrawableLines *>(
    //     Lines::create(Lines::LinesType::PRIMITIVE_LINES, "Spiral (triangulate version)")
    // );

    // primitive_line->setColor(1.0, 0.0, 1.0, 1.0);

    // primitive_line->beginLine();
    // primitive_line->addPoint(Lines::LinesPoint(-0.7f, -0.25f, 0.0f));
    // primitive_line->addPoint(Lines::LinesPoint(0.25f, -0.25f, 0.0f));
    // primitive_line->endLine();


    triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines*>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "")
    );

    triangulated_line->setColor(1.0, 0.0, 1.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(0.01f);
    triangulated_line->setThickness(0.01f);

    triangulated_line->beginLine();
    triangulated_line->addPoint(Lines::LinesPoint(-0.7f, -0.25f, 0.0f));
    triangulated_line->addPoint(Lines::LinesPoint(0.25f, -0.25f, 0.0f));
    triangulated_line->endLine();

    bgfx::touch(viewId());
}

HelloTriangleGLFW::~HelloTriangleGLFW()
{
}

void HelloTriangleGLFW::draw()
{
    primitive_line->draw(viewId());
}
