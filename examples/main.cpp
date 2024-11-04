#include <vclib/glfw/viewer_window.h>
#include <lines.hpp>

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    
    std::vector<lines::LinePoint> vertices = {
        lines::LinePoint(0.0f, 0.0f, 0.0f),
        lines::LinePoint(0.25f, 0.0f, -0.5f)
    };

    lines::TriangulatedLines line = lines::TriangulatedLines(vertices, tw.width(), tw.height());
    line.setThickness(3);
    tw.pushDrawableObject(line);

    tw.fitScene();

    tw.show();

    return 0;
}
