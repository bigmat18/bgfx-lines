#include <vclib/ext/glfw/viewer_window.h>
#include <iostream>

int main(void) {

    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    tw.fitScene();

    tw.show();

    return 0;
}