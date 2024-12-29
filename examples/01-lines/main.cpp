#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    // vcl::Context::instance().requestViewId();
    
    std::vector<lines::Segment> segments = {
        lines::Segment(
            lines::Point(0.5, 0.0f, 0.0f),
            lines::Point(1.0, 0.0f, 0.0f),
            lines::Color(0.0f, 1.0f, 1.0f, 1.0f)
        ),
        // lines::Segment(
        //     lines::Point(0.0f, -1.0f, 0.0f),
        //     lines::Point(0.0f, 1.0f, 0.0f),
        //     lines::Color(1.0f, 0.0f, 0.0f, 1.0f)
        // ),
        // lines::Segment(
        //     lines::Point(2.0f, -1.0f, 0.0f),
        //     lines::Point(3.0f, -1.0f, 0.0f),
        //     lines::Color(1.0f, 0.0f, 0.0f, 1.0f)
        // )
    }; 

    // std::vector<lines::Segment> segments;
    // generateSegmentsInCube(segments, 3, 100);

    auto line = lines::Lines::create(segments, tw.width(), tw.height());
    line->setThickness(20);
    // line->setLeftCap(lines::Caps::SQUARE_CAP);
    line->setRigthCap(lines::Caps::ROUND_CAP);

    // std::vector<lines::Segment> segments1;
    // generateSegmentsInCube(segments1, 3, 10000);
    // line->update(segments1);
    
    tw.pushDrawableObject(*line.get());    
    tw.fitScene();

    tw.show();

    return 0;
}
