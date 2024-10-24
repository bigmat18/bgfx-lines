#include <vclib/glfw/viewer_window.h>
#include "common.h"

int main(void) {

    vcl::glfw::ViewerWindow tw("Lines Viewer");

    #if 1
    Lines::TriangulatedDrawableLines *line1 = createTriangleLines(tw.width(), tw.height());
    tw.pushDrawableObject(*line1);
    #endif
        
    #if 0
    Lines::TriangulatedDrawableLines *line2 = createSpiralLines(tw.width(), tw.height());
    tw.pushDrawableObject(*line2);
    #endif

    #if 0
    Lines::TriangulatedDrawableLines *line3 = createSphereLines(tw.width(), tw.height());
    tw.pushDrawableObject(*line3);
    #endif

    tw.fitScene();

    tw.show();

    return 0;
}

