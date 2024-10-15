#include <vclib/ext/glfw/viewer_window.h>
#include <iostream>
#include <lines.hpp>
#include <primitive_drawable_lines.hpp>

int main(void) {

    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    Lines::PrimitiveDrawableLines *primitive_line = dynamic_cast<Lines::PrimitiveDrawableLines *>(
        Lines::create(Lines::LinesType::PRIMITIVE_LINES, "Spiral (triangulate version)")
    );

    primitive_line->setColor(1.0, 1.0, 1.0, 1.0);
    primitive_line->setClosed(true);

    primitive_line->beginLine();
    primitive_line->addPoint(Lines::LinesPoint(-0.25f, -0.25f, 0.0f));
    primitive_line->addPoint(Lines::LinesPoint(0.25f, -0.25f, 0.0f));
    primitive_line->addPoint(Lines::LinesPoint(0.0f, 0.25f, 0.0f));
    primitive_line->endLine();

    tw.pushDrawableObject(*primitive_line);

    tw.fitScene();

    tw.show();

    return 0;
}