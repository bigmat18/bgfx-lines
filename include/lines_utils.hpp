#pragma once
#include <bgfx_utils.h>
#include <iostream>
#include <stdint.h>
#include <cmath>

namespace lines {

    enum class LinesTypes {
        PRIMITIVE,
        CPU_GENERATED,
        GPU_GENERATED,
        INSTANCING_BASED,
        INDIRECT_BASED,
        TEXTURE_BASED,
    };

    inline bgfx::ProgramHandle LoadProgram(const std::string& vs_name, const std::string& fs_name) {
        return loadProgram(vs_name.c_str(), fs_name.c_str());
    }

    inline bgfx::ShaderHandle LoadShader(const std::string& name) {
        return loadShader(name.c_str());
    }

    struct LinesVertex {
        float X, Y, Z;
        float color;
        float xN, yN, zN;

        LinesVertex(float x, float y, float z, 
                    uint32_t color = COLOR(1, 1, 1, 1), 
                    float xn = 0, float yn = 0, float zn = 0) :
            X(x), Y(y), Z(z),
            color(std::bit_cast<float>(color)),
            xN(xn), yN(yn), zN(zn) {}

        uint32_t getRGBAColor() const { return std::bit_cast<uint32_t>(color); }

        uint32_t getABGRColor() const
        {
            uint32_t uint_color = getRGBAColor();
    
            return ((uint_color & 0xFF000000) >> 24) |
                   ((uint_color & 0x00FF0000) >> 8) |
                   ((uint_color & 0x0000FF00) << 8) |
                   ((uint_color & 0x000000FF) << 24);
        }

        static inline uint32_t COLOR(float r, float g, float b, float a) {
            return static_cast<uint32_t>(
                    (static_cast<uint8_t>(std::round((r) * 255)) << 24) |
                    (static_cast<uint8_t>(std::round((g) * 255)) << 16) |
                    (static_cast<uint8_t>(std::round((b) * 255)) << 8)  | 
                    (static_cast<uint8_t>(std::round((a) * 255)))
                );
        }
    };
}