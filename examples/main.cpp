#include <vclib/glfw/viewer_window.h>
#include "common.h"

int main(void)
{

    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    Lines::TriangulatedDrawableLines *line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Spiral (triangulate version)"));

    line->setColor(0.5, 1.0, 0.5, 1.0);
    line->setResolution(tw.width(), tw.height());
    line->setAntialis(0.0f);
    line->setThickness(0.01f);
    line->setClosed(true);

    line->beginLine();
    line->addPoint(Lines::LinesPoint(0.0f, 0.25f, 0.0f));
    line->addPoint(Lines::LinesPoint(-0.25f, -0.25f, 0.0f));
    line->addPoint(Lines::LinesPoint(0.25f, -0.25f, 0.0f));
    line->endLine();

    tw.pushDrawableObject(*line);

    tw.fitScene();

    tw.show();

    return 0;
}

