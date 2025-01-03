#include <vclib/glfw/viewer_window.h>
#include <polylines.hpp>

#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    std::vector<lines::Point> points = {
        lines::Point(0.0f, 0.0f, 0.0f),
        lines::Point(0.5f, 0.0f, 0.0f),
        lines::Point(1.0f, 1.0f, 0.0f),
        lines::Point(1.0f, 1.5f, 0.0f),
    };

    // std::vector<lines::Point> points;
    // generateSegmentsInCube(points, 3, 1000);

    auto line = lines::Polylines::create(points, tw.width(), tw.height(), lines::Types::GPU_GENERATED);
    line->setThickness(5);
    line->setMiterLimit(10);
    line->setColor(lines::Color(1.0, 0.0, 0.0, 1.0));
    // line->setJoin(lines::Joins::ROUND_JOIN);
    line->setLeftCap(lines::Caps::ROUND_CAP);
    line->setRigthCap(lines::Caps::ROUND_CAP);

    // std::vector<lines::Point> points2 = {
    //     lines::Point(0.0f, 0.0f, 1.0f),
    //     lines::Point(0.5f, 0.0f, 1.0f),
    //     lines::Point(1.0f, 1.0f, 1.0f),
    //     // lines::Point(1.0f, 1.0f, 0.0f),
    // };

    // auto line2 = lines::Polylines::create(points2, tw.width(), tw.height(), lines::Types::CPU_GENERATED);
    // line2->setThickness(20);
    // line2->setMiterLimit(50);
    // line2->setColor(lines::Color(1.0, 0.0, 0.0, 1.0));

    // std::vector<lines::Point> points1;
    // generateSegmentsInCube(points1, 3, 1000);
    // line->update(points1);
    
    tw.pushDrawableObject(*line.get());
    // tw.pushDrawableObject(*line2.get());

    tw.fitScene();

    tw.show();

    return 0;
}