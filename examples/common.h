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

Lines::TriangulatedDrawableLines* createSpiralLines(float width, float heigth) {
    Lines::TriangulatedDrawableLines *triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Spiral (triangulate version)")
    );

    triangulated_line->setColor(0.0, 0.0, 0.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(2.0f);
    triangulated_line->setThickness(3.0f);

    int n = 2048;
    std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
    std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);

    triangulated_line->beginLine();
    for (int i = 0; i < n; i++) {
        float X = cosf(T[i]) * R[i];
        float Y = sinf(T[i]) * R[i];

        triangulated_line->addPoint(Lines::LinesPoint(X, Y, 0.0));
    }
    triangulated_line->endLine();

    return triangulated_line;
}

Lines::TriangulatedDrawableLines* createSphereLines(float width, float heigth) {

    Lines::TriangulatedDrawableLines *triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Sphere (triangulate version)")
    );

    triangulated_line->setColor(0.0, 0.0, 0.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(3.0f);
    triangulated_line->setThickness(10.0f);

    int n = 10;

    std::vector<float> T = linespace(n, 0, 20 * 2 * M_PI);
    std::vector<float> R = linespace(n, 0.1, M_PI - 0.1);
    
    triangulated_line->beginLine();
    // for (int i = 0; i < n; i++) {
    //     float X = cosf(T[i]) * sinf(R[i]);
    //     float Y = sinf(T[i]) * sinf(R[i]);
    //     float Z = cosf(R[i]);

    //     triangulated_line->addPoint(Lines::LinesPoint(X, Y, Z));
    // }
    triangulated_line->addPoint(Lines::LinesPoint(0.0, -1.5, 0.0));
    triangulated_line->addPoint(Lines::LinesPoint(1.0, -1.0, 0.0));
    triangulated_line->endLine();

    return triangulated_line;
}

Lines::TriangulatedDrawableLines* createStarLines(float width, float heigth) {
    Lines::TriangulatedDrawableLines *line = dynamic_cast<Lines::TriangulatedDrawableLines*>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "")
    );
    line->setColor(0.0, 0.0, 0.0, 1.0);
    line->setResolution(width, heigth);
    line->setAntialis(0.0f);
    line->setThickness(10.0f);
    line->setClosed(true);

    float inner = 0.45, outer = 1.0, n = 5;
    std::vector<float> R(2 * n);
    for (int i = 0; i < n; ++i) {
        R[2 * i] = inner;
        R[2 * i + 1] = outer;
    }

    std::vector<float> T = linespace(2 * n, 0, 2 * M_PI);

    line->beginLine();
    for (int i = 0; i < 2 * n; ++i) {
        float X = R[i] * std::cos(T[i]);
        float Y = R[i] * std::sin(T[i]);
        line->addPoint(Lines::LinesPoint(X, Y, 0.0f));
    }
    line->endLine();
    return line;
}

Lines::TriangulatedDrawableLines* createTriangleLines(float width, float heigth) {
    Lines::TriangulatedDrawableLines *triangulated_line = dynamic_cast<Lines::TriangulatedDrawableLines *>(
        Lines::create(Lines::LinesType::TRIANGULATED_LINES, "Triangle (triangulate version)")
    );

    triangulated_line->setColor(0.0, 0.0, 0.0, 1.0);
    triangulated_line->setResolution(width, heigth);
    triangulated_line->setAntialis(0.0f);
    triangulated_line->setThickness(6.0f);
    triangulated_line->setClosed(false);

    triangulated_line->beginLine();
    triangulated_line->addPoint(Lines::LinesPoint(0.0, -0.5, 0.0));
    triangulated_line->addPoint(Lines::LinesPoint(1.0, 0.0, 0.0));
    // triangulated_line->addPoint(Lines::LinesPoint(0.0, 0.5, 0.0));
    triangulated_line->endLine();

    return triangulated_line;
}
