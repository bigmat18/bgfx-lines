#include <vclib/glfw/viewer_window.h>
#include <lines.hpp>

int main(void) {

    vcl::glfw::ViewerWindow tw("Lines Viewer");

    std::vector<lines::LinePoint> vertices = {
        lines::LinePoint(-.5, 0.0, 0.0),
        lines::LinePoint(.5, 0.0, 0.0)
    };

    lines::TriangulatedLines line(vertices, tw.width(), tw.height());
    line.setThickness(10.0f);
    tw.pushDrawableObject(line);

    tw.fitScene();

    tw.show();

    return 0;
}
