#include <vclib/glfw/viewer_window.h>
#include <polylines.hpp>

#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    // std::vector<lines::Point> points = {
    //     lines::Point(0.0f, 0.0f, 0.0f),
    //     lines::Point(1.0f, 0.0f, 0.0f),
    //     lines::Point(-0.5f, 0.5f, 0.0f),
    //     lines::Point(1.0f, 1.0f, 0.0f),
    // };

    std::vector<lines::Point> points;
    generateSegmentsInCube(points, 3, 10);

    auto line = lines::Polylines::create(points, tw.width(), tw.height());
    line->setThickness(20);
    line->setColor(lines::Color(1.0, 0.0, 0.0, 1.0));

    tw.pushDrawableObject(*line.get());

    tw.fitScene();

    tw.show();

    return 0;
}