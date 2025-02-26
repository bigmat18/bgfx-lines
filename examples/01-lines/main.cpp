#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include <lines/cpu_generated_lines.hpp>
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
    // vcl::glfw::ViewerWindow tw("Viewer GLFW");
    // vcl::Context::instance().requestViewId();
    
    // std::vector<lines::LinesVertex> points = {
    //     lines::LinesVertex(0.0, 0.0f, 0.0f, lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
    //     lines::LinesVertex(0.5, 0.0f, 0.0f, lines::LinesVertex::COLOR(1.0, 1.0, 0.0, 1.0)),
    //     // lines::LinesVertex(1.0f, -1.0f, 0.0f, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
    //     // lines::LinesVertex(1.0f, 1.0f, 0.25f, lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
    //     // lines::LinesVertex(1.5f, -1.0f, 0.0f, lines::LinesVertex::COLOR(1.0, 0.0, 0.5, 1.0)),
    //     // lines::LinesVertex(1.5f, 0.5f, 0.5f, lines::LinesVertex::COLOR(0.0, 1.0, 1.0, 1.0)),
    //     // lines::LinesVertex(-0.5f, 1.0f, 0.0f, lines::LinesVertex::COLOR(1.0, 0.25, 0.5, 1.0)),
    //     // lines::LinesVertex(-0.5f, 0.5f, 0.5f, lines::LinesVertex::COLOR(0.0, 1.0, 1.0, 1.0)),
    // }; 

    std::vector<lines::LinesVertex> points;
    generatePointsInCube(points, 3, 1000000);

    auto line = lines::Lines::create(points, lines::LinesTypes::INDIRECT_BASED);
    line->getSettings().setThickness(10);
    line->getSettings().setLeftCap(lines::Caps::ROUND_CAP);
    line->getSettings().setRigthCap(lines::Caps::ROUND_CAP);
    line->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
    line->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0, 0, 0, 1));
    tw.pushDrawableObject(*line.get());

    // line->getSettings().setBorder(1);
    // line->getSettings().setBorderColor(lines::LinesVertex::COLOR(1, 0, 0, 1));

    // std::vector<lines::LinesVertex> points1;
    // generatePointsInCube(points1, 3, 10000);
    // line->update(points1);

    // auto line2 = lines::Lines::create(points, lines::LinesTypes::PRIMITIVE);
    
    // tw.pushDrawableObject(*line.get());    
    // tw.pushDrawableObject(*line2.get());
    tw.fitScene();

    tw.show();

    return 0;
}