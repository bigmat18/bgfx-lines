#include <vclib/glfw/viewer_window.h>
#include <polylines.hpp>

#include "common.h"

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <imgui.h>

template<typename DerivedRenderApp>
class DemoImGuiDrawer : public vcl::imgui::ImGuiDrawer<DerivedRenderApp>
{
    using ParentDrawer = vcl::imgui::ImGuiDrawer<DerivedRenderApp>;

public:
    using ParentDrawer::ParentDrawer;

    virtual void onDraw(vcl::uint viewId) override
    {
        // draw the scene
        ParentDrawer::onDraw(viewId);

        if (!ParentDrawer::isWindowMinimized()) {
            // imgui demo window
            ImGui::ShowDemoWindow();
        }
    }
};

int main(int argc, char** argv)
{
    using ImGuiDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoImGuiDrawer,
        vcl::ViewerDrawer>;

    ImGuiDemo tw("Viewer ImGui GLFW");

    std::vector<lines::LinesVertex> points = {
        lines::LinesVertex(-2, 0.2, 0),
        lines::LinesVertex(-1.5, -0.2, 0),
        lines::LinesVertex(-1, 0.2, 0),
        lines::LinesVertex(-0.5, -0.2, 0),
        lines::LinesVertex(-0.0, 0.2, 0),
        lines::LinesVertex(0.5, -0.2, 0),
        lines::LinesVertex(1.0, 0.2, 0),
        lines::LinesVertex(1.5, -0.2, 0),
        lines::LinesVertex(2, 0.2, 0),
    };
    // lines::LinesVertex P0(0.0f, 0.0f, 0);
    // lines::LinesVertex P1(1.0f, 2.0f, 0);
    // lines::LinesVertex P2(2.0f, 0.0f, 0);
    // generateBezierCurve(points, P0, P1, P2, 100);

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