#include <vclib/glfw/viewer_window.h>
#include <lines.hpp>
#include <random>
#include <cmath>

#include "common.h"

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");
    
    std::vector<lines::Segment> segments;
    generateSegmentsInSphere(segments, 2.0f, 1000);

    auto line = lines::Lines::create(segments, tw.width(), tw.height());
    line->setThickness(3);
    tw.pushDrawableObject(*line.get());

    tw.fitScene();

    tw.show();

    return 0;
}
