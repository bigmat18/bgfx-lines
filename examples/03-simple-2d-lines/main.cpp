#include <vclib/glfw/viewer_window.h>
#include <vclib/bgfx/context.h>
#include <lines.hpp>
#include <lines/cpu_generated_lines.hpp>

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

    // std::vector<lines::LinesVertex> points0 = {
    //     lines::LinesVertex(-0.5, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
    //     lines::LinesVertex(-0.4, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
    // };

    // auto line0 = lines::Lines::create(points0, lines::LinesTypes::TEXTURE_BASED);
    // line0->getSettings().setThickness(1);
    // line0->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
    // line0->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
    // line0->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
    // tw.pushDrawableObject(*line0.get());    

// //===========

//     std::vector<lines::LinesVertex> points1 = {
//         lines::LinesVertex(-0.4, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(-0.3, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line1 = lines::Lines::create(points1, lines::LinesTypes::TEXTURE_BASED);
//     line1->getSettings().setThickness(2);
//     line1->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line1->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line1->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line1.get());

// //===========


//     std::vector<lines::LinesVertex> points2 = {
//         lines::LinesVertex(-0.3, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(-0.2, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line2 = lines::Lines::create(points2, lines::LinesTypes::TEXTURE_BASED);
//     line2->getSettings().setThickness(3);
//     line2->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line2->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line2->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line2.get());

// //===========


//     std::vector<lines::LinesVertex> point3 = {
//         lines::LinesVertex(-0.2, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(-0.1, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line3 = lines::Lines::create(point3, lines::LinesTypes::TEXTURE_BASED);
//     line3->getSettings().setThickness(4);
//     line3->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line3->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line3->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line3.get());

// //===========


//     std::vector<lines::LinesVertex> points4 = {
//         lines::LinesVertex(-0.1, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(0.0, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line4 = lines::Lines::create(points4, lines::LinesTypes::TEXTURE_BASED);
//     line4->getSettings().setThickness(5);
//     line4->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line4->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line4->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line4.get());


// //===========


//     std::vector<lines::LinesVertex> points5 = {
//         lines::LinesVertex(0.0, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(0.1, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line5 = lines::Lines::create(points5, lines::LinesTypes::TEXTURE_BASED);
//     line5->getSettings().setThickness(6);
//     line5->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line5->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line5->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line5.get());

// //===========


//     std::vector<lines::LinesVertex> points6 = {
//         lines::LinesVertex(0.1, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(0.2, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line6 = lines::Lines::create(points6, lines::LinesTypes::TEXTURE_BASED);
//     line6->getSettings().setThickness(7);
//     line6->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line6->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line6->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line6.get());

// //===========


//     std::vector<lines::LinesVertex> points7 = {
//         lines::LinesVertex(0.2, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(0.3, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line7 = lines::Lines::create(points7, lines::LinesTypes::TEXTURE_BASED);
//     line7->getSettings().setThickness(8);
//     line7->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line7->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line7->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line7.get());

// //===========


//     std::vector<lines::LinesVertex> points8 = {
//         lines::LinesVertex(0.3, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(0.4, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line8 = lines::Lines::create(points8, lines::LinesTypes::TEXTURE_BASED);
//     line8->getSettings().setThickness(9);
//     line8->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line8->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line8->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line8.get());

// //===========


//     std::vector<lines::LinesVertex> points9 = {
//         lines::LinesVertex(0.4, -0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//         lines::LinesVertex(0.5, 0.5, 0, lines::LinesVertex::COLOR(0.0, 0.0, 0.0, 1.0)),
//     };

//     auto line9 = lines::Lines::create(points9, lines::LinesTypes::TEXTURE_BASED);
//     line9->getSettings().setThickness(10);
//     line9->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
//     line9->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
//     line9->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
//     tw.pushDrawableObject(*line9.get());


//===========

    std::vector<lines::LinesVertex> points1 = {
        lines::LinesVertex(-0.4, -0.2, 0, lines::LinesVertex::COLOR(1.0, 0.0, 0.0, 1.0)),
        lines::LinesVertex(-0.3, 0.2, 0, lines::LinesVertex::COLOR(0.0, 1.0, 0.0, 1.0)),
    };

    auto line1 = lines::Lines::create(points1, lines::LinesTypes::TEXTURE_BASED);
    line1->getSettings().setThickness(30);
    line1->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
    line1->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
    line1->getSettings().setColorToUse(lines::ColorToUse::PER_VERTEX_COLOR);
    line1->getSettings().setBorder(3);
    // tw.pushDrawableObject(*line1.get());

//===========


    std::vector<lines::LinesVertex> points2 = {
        lines::LinesVertex(-0.2, -0.2, 0, lines::LinesVertex::COLOR(0.0, 0.0, 1.0, 1.0)),
        lines::LinesVertex(-0.1, 0.2, 0, lines::LinesVertex::COLOR(1.0, 0.0, 1.0, 1.0)),
    };

    auto line2 = lines::Lines::create(points2, lines::LinesTypes::TEXTURE_BASED);
    line2->getSettings().setThickness(30);
    line2->getSettings().setLeftCap(lines::Caps::ROUND_CAP);
    line2->getSettings().setRigthCap(lines::Caps::ROUND_CAP);
    line2->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line2->getSettings().setBorder(5);
    line2->getSettings().setBorderColor(lines::LinesVertex::COLOR(1.0, 0.0, 1.0, 1.0));
    tw.pushDrawableObject(*line2.get());

//===========


    std::vector<lines::LinesVertex> point3 = {
        lines::LinesVertex(0, -0.2, 0, lines::LinesVertex::COLOR(0.0, 0.5, 0.5, 1.0)),
        lines::LinesVertex(0.1, 0.2, 0, lines::LinesVertex::COLOR(1.0, 0.0, 0.0, 1.0)),
    };

    auto line3 = lines::Lines::create(point3, lines::LinesTypes::TEXTURE_BASED);
    line3->getSettings().setThickness(30);
    line3->getSettings().setLeftCap(lines::Caps::SQUARE_CAP);
    line3->getSettings().setRigthCap(lines::Caps::SQUARE_CAP);
    line3->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line3->getSettings().setBorder(5);
    line3->getSettings().setBorderColor(lines::LinesVertex::COLOR(0.0, 0.5, 1.0, 1.0));
    tw.pushDrawableObject(*line3.get());

//===========


    std::vector<lines::LinesVertex> points4 = {
        lines::LinesVertex(0.2, -0.2, 0, lines::LinesVertex::COLOR(0.5, 1.0, 0.0, 1.0)),
        lines::LinesVertex(0.3, 0.2, 0, lines::LinesVertex::COLOR(0.0, 0.5, 0.1, 1.0)),
    };

    auto line4 = lines::Lines::create(points4, lines::LinesTypes::TEXTURE_BASED);
    line4->getSettings().setThickness(30);
    line4->getSettings().setLeftCap(lines::Caps::TRIANGLE_CAP);
    line4->getSettings().setRigthCap(lines::Caps::TRIANGLE_CAP);
    line4->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line4->getSettings().setBorder(5);
    line4->getSettings().setBorderColor(lines::LinesVertex::COLOR(1.0, 0.0, 0, 1.0));

    tw.pushDrawableObject(*line4.get());

    tw.fitScene();

    tw.show();

    return 0;
}