#pragma once

namespace lines {

    struct Point {
        float x, y, z;

        Point(float xx, float yy, float zz)
            : x(xx), y(yy), z(zz) {}
    };

    struct Color {
        float r, g, b, a;

        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

        Color(float rr, float gg, float bb, float aa)
            : r(rr), g(gg), b(bb), a(aa) {}
    };

    struct Segment {
        Point m_P0;
        Point m_P1;
	    Color m_Color;

        Segment(const Point p0, const Point p1, const Color color) :
            m_P0(p0), m_P1(p1), m_Color(color) {}
    };

    enum Types {
        CPU_GENERATED,
        GPU_GENERATED,
        INSTANCING_BASED,
        TEXTURE_BASED
    };
}