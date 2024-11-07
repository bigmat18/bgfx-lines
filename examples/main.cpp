#include <vclib/glfw/viewer_window.h>
#include <lines.hpp>

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    
    std::vector<lines::LinesPoint> points = {
        lines::LinesPoint(0.0f, 0.0f, 0.0f),
        lines::LinesPoint(0.25f, 0.0f, -0.5f)
    };

    auto line = lines::Lines::create(points, tw.width(), tw.height(), lines::LinesType::GPU_GENERATED_LINES);
    line->setThickness(3);
    tw.pushDrawableObject(*line.get());

    tw.fitScene();

    tw.show();

    return 0;
}
