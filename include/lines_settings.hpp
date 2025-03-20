#pragma once
#include <iostream>
#include <assert.h>
#include <bgfx/bgfx.h>
#include "utils.hpp"

namespace lines {

    enum class LineCap {
        BUTT_CAP     = 0x00000000, // No extra space on caps
        SQUARE_CAP   = 0x00000001, // Extra space with square shape
        ROUND_CAP    = 0x00000002, // Extra space with round shape
        TRIANGLE_CAP = 0x00000003  // Extra space with triangle
    };
    
    enum class LineColorToUse {
        PER_VERTEX_COLOR = 0x00000000, // Select color form vertex color
        PER_EDGE_COLOR   = 0x00000001, // Select color from edge buffer color
        GENERAL_COLOR    = 0x00000002  // Use general color in uniform data
    };
    
    class LinesSettings
    {
    protected:
        uint8_t  mThickness = 5;
        uint8_t  mAntialias = 0;
        uint8_t  mBorder = 0;
        uint32_t mBorderColor = LinesVertex::COLOR(0, 0, 0, 1);
        uint32_t mGeneralColor = LinesVertex::COLOR(0, 0, 0, 1);
    
        LineCap        mLeftCap    = LineCap::ROUND_CAP;
        LineCap        mRigthCap   = LineCap::ROUND_CAP;
        LineColorToUse mColorToUse = LineColorToUse::PER_VERTEX_COLOR;
    
        bgfx::UniformHandle mDataUH = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
    
    public:
        LinesSettings() = default;
    
        void setThickness(uint8_t thickness) { mThickness = thickness; }
    
        void setAntialias(uint8_t antialias) { mAntialias = antialias; }
    
        void setBorder(uint8_t border) { mBorder = border; }
    
        void setBorderColor(uint32_t borderColor) { mBorderColor = borderColor; }
    
        void setGeneralColor(uint32_t generalColor)
        {
            mGeneralColor = generalColor;
        }
    
        void setLeftCap(LineCap cap) { mLeftCap = cap; }
    
        void setRigthCap(LineCap cap) { mRigthCap = cap; }
    
        void setColorToUse(LineColorToUse colorToUse) { mColorToUse = colorToUse; }
    
        void bindUniform() const
        {
            uint32_t thickness_antialias_border_caps_color =
                (0 | mThickness << 24 | mAntialias << 16 | mBorder << 8 |
                 static_cast<uint8_t>(mLeftCap) << 4 |
                 static_cast<uint8_t>(mRigthCap) << 2 |
                 static_cast<uint8_t>(mColorToUse));

            uint32_t data[] = {
                mGeneralColor,
                thickness_antialias_border_caps_color,
                mBorderColor,
                0
            };
            bgfx::setUniform(mDataUH, data);
        }
    };
}