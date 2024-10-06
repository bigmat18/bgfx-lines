#pragma once
#include <memory>

namespace Lines
{
    enum class LinesType
    {
        TRIANGULATED_LINES,
        PRIMITIVE_LINES
    };

    struct LinesPoint
    {
        float x, y, z;

        LinesPoint(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    };

    class LinesHandler* CreateHandler(LinesType type, const std::string name);
    void DestroyHandler(class LinesHandler* handler);

    void Shutdown();
    void Render(uint64_t state);
    void DebugMenu();
}