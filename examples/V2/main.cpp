#include <vclib/glfw/viewer_window.h>

int main(void) {

    vcl::glfw::ViewerWindow tw("Lines Viewer");

    tw.fitScene();

    tw.show();

    return 0;
}
