#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    // vcl::Context::instance().requestViewId();
    
    std::vector<lines::LinesVertex> points = {
        lines::LinesVertex(0.0, 0.0f, 0.0f, lines::COLOR(0.0, 1.0, 0.0, 1.0)),
        lines::LinesVertex(0.5, 0.5f, 0.0f, lines::COLOR(1.0, 1.0, 0.0, 1.0)),
        // lines::LinesVertex(1.0f, -1.0f, 0.0f, lines::COLOR(0.0, 0.0, 0.0, 1.0)),
        // lines::LinesVertex(1.0f, 1.0f, 0.0f, lines::COLOR(0.0, 1.0, 0.0, 1.0)),
        // lines::LinesVertex(1.5f, -1.0f, 0.0f, lines::COLOR(1.0, 0.0, 0.5, 1.0)),
        // lines::LinesVertex(1.5f, 0.5f, 0.0f, lines::COLOR(0.0, 1.0, 1.0, 1.0)),
    }; 

    // std::vector<lines::LinesVertex> points;
    // generatePointsInSphere(points, 3, 100);

    auto line = lines::Lines::create(points, tw.width(), tw.height(), lines::Types::TEXTURE_BASED);
    line->getSettings().setThickness(5);
    line->getSettings().setLeftCap(lines::Caps::TRIANGLE_CAP);
    line->getSettings().setRigthCap(lines::Caps::TRIANGLE_CAP);
    line->getSettings().setBorder(1);


    // line->getSettings().setBorderColor(lines::COLOR(0.0, 0.0, 1.0, 1.0));

    // std::vector<lines::Segment> segments1;
    // generateSegmentsInCube(segments1, 3, 10000);
    // line->update(segments1);
    
    tw.pushDrawableObject(*line.get());    
    tw.fitScene();

    tw.show();

    return 0;
}
