#include <vclib/glfw/viewer_window.h>
#include <polylines.hpp>

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <imgui.h>

#include <vector>
#include <cstdlib>
#include <random>
#include <ctime> 
#include <cmath>

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


void generateFrequency(std::vector<lines::LinesVertex> &points, float minAmplitude, float maxAmplitude, float minFrequency, float maxFrequency, float sampleRate, float duration, float traslate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    auto randomFloat = [&gen](float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    };

    for (float x = -duration / 2; x <= duration / 2; x += sampleRate) {

        float amplitude = randomFloat(minAmplitude, maxAmplitude);
        float frequency = randomFloat(minFrequency, maxFrequency);

        float signal = 0.0f;
        for (int i = 0; i < 3; ++i) { // Usa 3 onde con fasi diverse
            float localFreq = randomFloat(minFrequency, maxFrequency);
            float localAmp = randomFloat(minAmplitude, maxAmplitude);
            signal += localAmp * std::sin(2 * M_PI * localFreq * x + randomFloat(0, 2 * M_PI));
        }

        points.push_back(lines::LinesVertex(x, traslate + (signal), 0));
    }
}

int main(int argc, char** argv)
{
    using ImGuiDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoImGuiDrawer,
        vcl::ViewerDrawer>;

    ImGuiDemo tw("Viewer ImGui GLFW");

    std::vector<lines::LinesVertex> points1;
    generateFrequency(points1, 0.5f, 1.0f, 0.1f, 2.0f, 0.05f, 15.0f, -5.0f);

    auto line1 = lines::Polylines::create(points1, lines::LinesTypes::TEXTURE_BASED);
    line1->getSettings().setThickness(5);
    line1->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line1->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0.5, 0, 0, 1));
    line1->getSettings().setJoin(lines::Joins::MITER_JOIN);
    tw.pushDrawableObject(*line1.get());

    std::vector<lines::LinesVertex> points2;
    generateFrequency(points2, 0.5f, 1.0f, 0.1f, 2.0f, 0.1f, 15.0f, 0.0f);
    auto line2 = lines::Polylines::create(points2, lines::LinesTypes::TEXTURE_BASED);
    line2->getSettings().setThickness(5);
    line2->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line2->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0, 0.5, 0, 1));
    line2->getSettings().setJoin(lines::Joins::MITER_JOIN);
    tw.pushDrawableObject(*line2.get());

    std::vector<lines::LinesVertex> points3;
    generateFrequency(points3, 0.5f, 1.0f, 0.1f, 2.0f, 0.15f, 15.0f, 5.0f);
    auto line3 = lines::Polylines::create(points3, lines::LinesTypes::TEXTURE_BASED);
    line3->getSettings().setThickness(5);
    line3->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line3->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0, 0, 0.5, 1));
    line3->getSettings().setJoin(lines::Joins::MITER_JOIN);
    tw.pushDrawableObject(*line3.get());

    tw.fitScene();

    tw.show();

    return 0;
}