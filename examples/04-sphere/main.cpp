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

inline std::vector<float> linespace(int n, float start, float stop) {
    std::vector<float> T(n);
    float step = (stop - start) / (n - 1);

    for (int i = 0; i < n; ++i)
        T[i] = start + i * step;
    return T;
}

void createSphereLines(std::vector<lines::LinesVertex> &points, int n) {

    std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
    std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);
    
    for (int i = 0; i < n; i++) {
        float X1 = cosf(T[i]) * sinf(R[i]);
        float Y1 = sinf(T[i]) * sinf(R[i]);
        float Z1 = cosf(R[i]);

        points.push_back(lines::LinesVertex(X1, Y1, Z1));
    }
}

void generateTourus(std::vector<lines::LinesVertex> &points, float R, float r, int numMajor, int numMinor) {
    std::vector<lines::LinesVertex> vertices;
    
    constexpr float TWO_PI = 2.0f * M_PI;

    // Generazione dei punti sulla superficie del toro
    for (int i = 0; i < numMajor; ++i) {
        float majorAngle = i * TWO_PI / numMajor;

        for (int j = 0; j < numMinor; ++j) {
            float minorAngle = j * TWO_PI / numMinor;

            // Calcola le coordinate del punto
            float x = (R + r * cos(minorAngle)) * cos(majorAngle);
            float y = (R + r * cos(minorAngle)) * sin(majorAngle);
            float z = r * sin(minorAngle);

            vertices.emplace_back(x, y, z);
        }
    }

    // Creazione dei segmenti collegando i punti adiacenti
    std::vector<lines::LinesVertex> segments;

    for (int i = 0; i < numMajor; ++i) {
        for (int j = 0; j < numMinor; ++j) {
            int current = i * numMinor + j;
            int nextMinor = i * numMinor + (j + 1) % numMinor; // Prossimo nella stessa sezione
            int nextMajor = ((i + 1) % numMajor) * numMinor + j; // Prossimo anello

            // Segmento lungo la circonferenza minore
            segments.push_back(vertices[current]);
            segments.push_back(vertices[nextMinor]);

            // Segmento lungo la circonferenza maggiore
            segments.push_back(vertices[current]);
            segments.push_back(vertices[nextMajor]);
        }
    }
    
    points.assign(segments.begin(), segments.end());

}

int main(int argc, char** argv)
{
    using ImGuiDemo = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        DemoImGuiDrawer,
        vcl::ViewerDrawer>;

    ImGuiDemo tw("Viewer ImGui GLFW");

    std::vector<lines::LinesVertex> points;
    generateTourus(points, 10, 20, 100, 100);

    auto line = lines::Lines::create(points, lines::LinesTypes::TEXTURE_BASED);
    line->getSettings().setThickness(5);
    line->getSettings().setLeftCap(lines::Caps::BUTT_CAP);
    line->getSettings().setRigthCap(lines::Caps::BUTT_CAP);
    line->getSettings().setColorToUse(lines::ColorToUse::GENERAL_COLOR);
    line->getSettings().setGeneralColor(lines::LinesVertex::COLOR(0, 0, 0, 1));
    tw.pushDrawableObject(*line.get());

    tw.fitScene();

    tw.show();

    return 0;
}