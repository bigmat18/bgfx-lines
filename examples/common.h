#include <lines.hpp>
#include <primitive_drawable_lines.hpp>
#include <triangulated_drawable_lines.hpp>
#include <vector>

inline std::vector<float> linespace(int n, float start, float stop) {
    std::vector<float> T(n);
    float step = (stop - start) / (n - 1);

    for (int i = 0; i < n; ++i)
        T[i] = start + i * step;

    return T;
}

void createSpiralLines(float width, float heigth) {
    Lines::TriangulatedDrawableLines *triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Spiral (triangulate version)"));
    triangulated_line->setColor(1.0, 0.0, 1.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(0.01f);
    triangulated_line->setThickness(0.01f);

    Lines::PrimitiveDrawableLines *primitive_line = dynamic_cast<Lines::PrimitiveDrawableLines *>(
        Lines::create(Lines::LinesType::PRIMITIVE_LINES, "Spiral (primitive version)")

    );

    primitive_line->setColor(1.0, 0.0, 1.0, 1.0);

    triangulated_line->beginLine();
    primitive_line->beginLine();

    int n = 2048;
    std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
    std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);

    for (int i = 0; i < n; i++)
    {
        float X = cosf(T[i]) * R[i];
        float Y = sinf(T[i]) * R[i];

        triangulated_line->addPoint(Lines::LinesPoint(X, Y, -3.0));
        primitive_line->addPoint(Lines::LinesPoint(X, Y, -3.0));
    }

    triangulated_line->endLine();
    primitive_line->endLine();
}

void createSphereLines(float width, float heigth) {
    Lines::TriangulatedDrawableLines *triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Sphere (triangulate version)"));
    triangulated_line->setColor(1.0, 0.0, 1.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(0.001f);
    triangulated_line->setThickness(0.007f);

    Lines::PrimitiveDrawableLines *primitive_line = dynamic_cast<Lines::PrimitiveDrawableLines *>(
        Lines::create(Lines::LinesType::PRIMITIVE_LINES, "Sphere (primitive version)"));

    primitive_line->setColor(1.0, 0.0, 1.0, 1.0);

    triangulated_line->beginLine();
    primitive_line->beginLine();

    int n = 2048;

    std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
    std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);

    for (int i = 0; i < n; i++)
    {
        float X = cosf(T[i]) * sinf(R[i]);
        float Y = sinf(T[i]) * sinf(R[i]);
        float Z = cosf(R[i]);

        triangulated_line->addPoint(Lines::LinesPoint(X, Y, -3.0f + Z));
        primitive_line->addPoint(Lines::LinesPoint(X, Y, Z));
    }

    triangulated_line->endLine();
    primitive_line->endLine();
}

void createTriangleLines(float width, float heigth) {
    Lines::TriangulatedDrawableLines *triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Triangle (triangulate version)"));

    triangulated_line->setColor(1.0, 0.0, 1.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(0.01f);
    triangulated_line->setThickness(0.01f);

    Lines::PrimitiveDrawableLines *primitive_line = dynamic_cast<Lines::PrimitiveDrawableLines *>(
        Lines::create(Lines::LinesType::PRIMITIVE_LINES, "Triangle (primitive version)"));

    primitive_line->setColor(1.0, 0.0, 1.0, 1.0);

    triangulated_line->beginLine();
    primitive_line->beginLine();

    primitive_line->setClosed(true);
    primitive_line->addPoint(Lines::LinesPoint(-0.25f, -0.25f, 0.0f));
    primitive_line->addPoint(Lines::LinesPoint(0.25f, -0.25f, 0.0f));
    primitive_line->addPoint(Lines::LinesPoint(0.0f, 0.25f, 0.0f));

    triangulated_line->setClosed(true);
    triangulated_line->addPoint(Lines::LinesPoint(-0.25, -0.25, 0.0));
    triangulated_line->addPoint(Lines::LinesPoint(0.25, -0.25, 0.0));
    triangulated_line->addPoint(Lines::LinesPoint(0.0, 0.25, 0.0));

    triangulated_line->endLine();
    primitive_line->endLine();
}
