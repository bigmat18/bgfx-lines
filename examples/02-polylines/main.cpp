#include <vclib/glfw/viewer_window.h>
#include <polylines.hpp>

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

    auto line1 = lines::Polylines::create(points, lines::LinesTypes::CPU_GENERATED);
    line1->getSettings().setThickness(30);
    line1->getSettings().setMiterLimit(60);
    line1->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
    line1->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
    line1->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line1->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0, 0, 0, 1));
    line1->getSettings().setJoin(lines::Joins::ROUND_JOIN);
    tw.pushDrawableObject(*line1.get());

    tw.fitScene();

    tw.show();

    return 0;
}