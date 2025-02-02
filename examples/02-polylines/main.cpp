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

    // std::vector<lines::LinesVertex> points = {
    //     lines::LinesVertex(0.0f, 0.0f, 0.0f, lines::LinesVertex::COLOR(1.0, 0.0, 0.0, 1.0)),
    //     lines::LinesVertex(0.5f, 0.0f, 0.0f, lines::LinesVertex::COLOR(0.0, 0.0, 1.0, 1.0)),
    //     lines::LinesVertex(1.0f, 1.0f, 0.0f, lines::LinesVertex::COLOR(1.0, 0.0, 0.0, 1.0)),
    //     // lines::LinesVertex(1.0f, 1.5f, 0.0f, lines::LinesVertex::COLOR(1.0, 0.0, 0.0, 1.0)),
    // };
    std::vector<lines::LinesVertex> points;
    generateSegmentsInCube(points, 3, 100);

    auto line = lines::Polylines::create(points, lines::LinesTypes::TEXTURE_BASED);
    line->getSettings().setThickness(10);
    line->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0, 0, 0, 1));
    // line->getSettings().setMiterLimit(20);
    // line->getSettings().setBorderColor(lines::LinesVertex::COLOR(1.0, 0.0, 1.0, 1.0));
    // line->getSettings().setBorder(2);
    // line->getSettings().setLeftCap(lines::Caps::TRIANGLE_CAP);
    // line->getSettings().setRigthCap(lines::Caps::ROUND_CAP);
    // line->getSettings().setJoin(lines::Joins::ROUND_JOIN);

    // line->setThickness(5);
    // line->setMiterLimit(10);

    // line->setJoin(lines::Joins::ROUND_JOIN);
    // line->setLeftCap(lines::Caps::ROUND_CAP);
    // line->setRigthCap(lines::Caps::ROUND_CAP);

    // std::vector<lines::Point> points2 = {
    //     lines::Point(0.0f, 0.0f, 1.0f),
    //     lines::Point(0.5f, 0.0f, 1.0f),
    //     lines::Point(1.0f, 1.0f, 1.0f),
    //     // lines::Point(1.0f, 1.0f, 0.0f),
    // };

    // auto line2 = lines::Polylines::create(points2, tw.width(), tw.height(), lines::Types::CPU_GENERATED);
    // line2->setThickness(20);
    // line2->setMiterLimit(50);
    // line2->setColor(lines::Color(1.0, 0.0, 0.0, 1.0));

    // std::vector<lines::LinesVertex> points1;
    // generateSegmentsInCube(points1, 3, 10);
    // line->update(points1);
    
    tw.pushDrawableObject(*line.get());
    // tw.pushDrawableObject(*line2.get());

    tw.fitScene();

    tw.show();

    return 0;
}