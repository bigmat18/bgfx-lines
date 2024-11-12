#include <vclib/glfw/viewer_window.h>
#include <polylines.hpp>

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    
    std::vector<lines::Point> points = {
        lines::Point(1.0f, 0.0f, 0.0f),
        lines::Point(1.0f, 1.0f, 0.0f),
        lines::Point(1.5f, 1.0f, 0.0f),
    };

    auto line = lines::Polylines::create(points, tw.width(), tw.height());
    line->setThickness(2);
    tw.pushDrawableObject(*line.get());

    tw.fitScene();

    tw.show();

    return 0;
}