#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include <lines/cpu_generated_lines.hpp>
#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    // vcl::Context::instance().requestViewId();
    
    std::vector<lines::LinesVertex> points = {
        lines::LinesVertex(0.0, 0.0f, -2.0f, lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
        lines::LinesVertex(0.5, 0.5f, 0.0f, lines::LinesVertex::COLOR(1.0, 1.0, 0.0, 1.0)),
        // lines::LinesVertex(1.0f, -1.0f, 0.0f, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
        // lines::LinesVertex(1.0f, 1.0f, 0.25f, lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
        // lines::LinesVertex(1.5f, -1.0f, 0.0f, lines::LinesVertex::COLOR(1.0, 0.0, 0.5, 1.0)),
        // lines::LinesVertex(1.5f, 0.5f, 0.5f, lines::LinesVertex::COLOR(0.0, 1.0, 1.0, 1.0)),
    }; 

    // std::vector<lines::LinesVertex> points;
    // generatePointsInCube(points, 3, 50);

    auto line = lines::Lines::create(points, lines::LinesTypes::CPU_GENERATED);
    // line->getSettings().setThickness(10);
    line->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
    line->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
    // line->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
    // line->getSettings().setBorder(2);
    // line->getSettings().setBorderColor(lines::LinesVertex::COLOR(1, 0, 0, 1));

    // std::vector<lines::LinesVertex> points1;
    // generatePointsInCube(points1, 3, 10000);
    // line->update(points1);

    auto line2 = lines::Lines::create(points, lines::LinesTypes::PRIMITIVE);
    
    tw.pushDrawableObject(*line.get());    
    tw.pushDrawableObject(*line2.get());
    tw.fitScene();

    tw.show();

    return 0;
}
