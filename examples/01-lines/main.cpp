#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    // vcl::Context::instance().requestViewId();
    
    std::vector<lines::Point> points = {
        lines::Point(0.0, 0.0f, 0.0f, lines::Color(1.0, 0.0, 0.0, 1.0)),
        lines::Point(0.5, 0.0f, 0.0f, lines::Color(1.0, 0.0, 0.0, 1.0)),
        lines::Point(1.0f, -1.0f, 0.0f, lines::Color(0.0, 1.0, 0.0, 1.0)),
        lines::Point(1.0f, 1.0f, 0.0f, lines::Color(0.0, 1.0, 0.0, 1.0)),
        lines::Point(1.5f, -1.0f, 0.0f, lines::Color(0.0, 0.0, 1.0, 1.0)),
        lines::Point(1.5f, 0.5f, 0.0f, lines::Color(0.0, 0.0, 1.0, 1.0)),
    }; 

    // std::vector<lines::Segment> points;
    // generateSegmentsInCube(segments, 3, 2);

    auto line = lines::Lines::create(points, tw.width(), tw.height(), lines::Types::TEXTURE_BASED);
    line->setThickness(5);
    line->setLeftCap(lines::Caps::ROUND_CAP);
    line->setRigthCap(lines::Caps::ROUND_CAP);

    // std::vector<lines::Segment> segments1;
    // generateSegmentsInCube(segments1, 3, 10000);
    // line->update(segments1);
    
    tw.pushDrawableObject(*line.get());    
    tw.fitScene();

    tw.show();

    return 0;
}
