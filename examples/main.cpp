#include <vclib/glfw/viewer_window.h>
#include "common.h"

int main(void) {

    vcl::glfw::ViewerWindow tw("Lines Viewer");

    Lines::TriangulatedDrawableLines *line = createTriangleLines(tw.width(), tw.height());

    tw.pushDrawableObject(*line);

    tw.fitScene();

    tw.show();

    return 0;
}

