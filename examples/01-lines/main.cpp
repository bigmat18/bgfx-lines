#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    // vcl::Context::instance().requestViewId();
    
    // std::vector<lines::LinesVertex> points = {
    //     lines::LinesVertex(0.0, 0.0f, 0.0f, lines::COLOR(1.0, 0.0, 0.0, 1.0)),
    //     lines::LinesVertex(0.5, 0.5f, 0.0f, lines::COLOR(0.0, 0.0, 1.0, 1.0)),
    //     lines::LinesVertex(1.0f, -1.0f, 0.0f, lines::COLOR(0.0, 1.0, 0.0, 1.0)),
    //     lines::LinesVertex(1.0f, 1.0f, 0.0f, lines::COLOR(0.0, 1.0, 0.0, 1.0)),
    //     lines::LinesVertex(1.5f, -1.0f, 0.0f, lines::COLOR(1.0, 0.0, 0.5, 1.0)),
    //     lines::LinesVertex(1.5f, 0.5f, 0.0f, lines::COLOR(0.0, 1.0, 1.0, 1.0)),
    // }; 

    std::vector<lines::LinesVertex> points;
    generatePointsInCube(points, 3, 500000);

    auto line = lines::Lines::create(points, tw.width(), tw.height(), lines::Types::TEXTURE_BASED);
    line->getSettings().setThickness(5);
    // line->getSettings().setBorderColor(lines::COLOR(0.0, 0.0, 1.0, 1.0));
    // line->setThickness(1);
    // line->setBorder(1);
    // line->setAntialias(2);

    // line->setLeftCap(lines::Caps::ROUND_CAP);
    // line->setRigthCap(lines::Caps::ROUND_CAP);
    // line->setBorderColor(lines::Color(0.0, 0.0, 1.0, 1.0));

    // std::vector<lines::Segment> segments1;
    // generateSegmentsInCube(segments1, 3, 10000);
    // line->update(segments1);
    
    tw.pushDrawableObject(*line.get());    
    tw.fitScene();

    tw.show();

    return 0;
}
