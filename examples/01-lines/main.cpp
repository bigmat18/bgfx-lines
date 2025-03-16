#include <vclib/glfw/viewer_window.h>
#include <lines.hpp>

#include "../common/generator.h"
#include "../common/demo_imgui.h"


int main(int argc, char** argv)
{
    using ImGuiDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoImGuiDrawer,
        vcl::ViewerDrawer>;

    ImGuiDemo tw("Viewer ImGui GLFW");

    std::vector<lines::LinesVertex> points;
    generatePointsInCube(points, 3, 1000);

    auto line = lines::Lines::create(points, lines::LinesTypes::GPU_GENERATED);
    line->getSettings().setThickness(10);
    tw.pushDrawableObject(*line.get());
    tw.fitScene();

    tw.show();

    return 0;
}