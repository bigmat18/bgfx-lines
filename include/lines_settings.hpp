#pragma once
#include <iostream>
#include <assert.h>
#include <bgfx/bgfx.h>
#include "utils.hpp"

namespace lines {

    enum Caps {
        BUTT_CAP            = 0x00000000, // No extra space on caps
        SQUARE_CAP          = 0x00000001, // Extra space with square shape
        ROUND_CAP           = 0x00000002, // Extra space with round shape
        TRIANGLE_CAP        = 0x00000003  // Extra space with triangle
    };

    enum Joins {
        ROUND_JOIN          = 0x00000000, // Join with round shape
        BEVEL_JOIN          = 0x00000001, // Join with square shape
        MITER_JOIN          = 0x00000002, // Join with a miter
    };

    enum ColorToUse {
        PER_VERTEX_COLOR    = 0x00000000, // Select color form vertex color
        PER_EDGE_COLOR      = 0x00000001, // Select color from edge buffer color
        GENERAL_COLOR       = 0x00000002  // Use general color in uniform data
    };

    class LinesSettings {
        private:
            uint8_t m_Thickness;
            uint8_t m_Antialias;
            uint8_t m_Border;
            uint32_t m_BorderColor;
            uint32_t m_GeneralColor;
            uint8_t m_MiterLimit;

            Caps m_LeftCap;
            Caps m_RigthCap;
            Joins m_Join;
            ColorToUse m_ColorToUse;

            bgfx::UniformHandle m_UniformData;

        public:

            LinesSettings() : 
                m_Thickness(5),
                m_Antialias(0),
                m_Border(0),
                m_BorderColor(LinesVertex::COLOR(0, 0, 0, 1)),
                m_GeneralColor(LinesVertex::COLOR(1, 0, 0, 1)),
                m_MiterLimit(m_Thickness * 2),
                m_LeftCap(Caps::ROUND_CAP),
                m_RigthCap(Caps::ROUND_CAP),
                m_Join(Joins::ROUND_JOIN),
                m_ColorToUse(ColorToUse::GENERAL_COLOR)
            {
                m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
            }

            ~LinesSettings() {
                if(bgfx::isValid(m_UniformData))
                    bgfx::destroy(m_UniformData);
            }

            Joins getJoin() const { return m_Join; }

            void setThickness(uint8_t thickness) { m_Thickness = thickness; }

            void setAntialias(uint8_t antialias) { m_Antialias = antialias; }

            void setBorder(uint8_t border) { m_Border = border; }

            void setBorderColor(uint32_t borderColor) { m_BorderColor = borderColor; }

            void setGeneralColor(uint32_t generalColor) { m_GeneralColor = generalColor; }

            void setMiterLimit(uint8_t miterLimit) {
                if(miterLimit < m_Thickness)
                    assert((void("Miter limit must be greatest then thickness * 2"), false));
                m_MiterLimit = miterLimit;
            }

            void setLeftCap(Caps cap) { m_LeftCap = cap; }

            void setRigthCap(Caps cap) { m_RigthCap = cap; }

            void setJoin(Joins join) { m_Join = join; }

            void setColorToUse(ColorToUse colorToUse) { m_ColorToUse = colorToUse; }

        public:
            void bindUniformLines() const {
                uint32_t thickness_antialias_border_caps_color = ( 
                    0                                             |
                    m_Thickness                             << 24 |
                    m_Antialias                             << 16 |
                    m_Border                                << 8  |
                    static_cast<uint8_t>(m_LeftCap)         << 4  |
                    static_cast<uint8_t>(m_RigthCap)        << 2  |
                    static_cast<uint8_t>(m_ColorToUse)  
                );
                uint32_t data[] = {m_GeneralColor, thickness_antialias_border_caps_color, m_BorderColor, 0};
                bgfx::setUniform(m_UniformData, data);
            }

            void bindUniformPolylines() const {
                uint32_t thickness_antialias_border_miterlimit = ( 
                    0                                       |
                    m_Thickness                       << 24 |
                    m_Antialias                       << 16 |
                    m_Border                          << 8  |
                    m_MiterLimit 
                );

                uint32_t caps_join_color = (                  
                   0                                          |
                   static_cast<uint8_t>(m_LeftCap)       << 6 |
                   static_cast<uint8_t>(m_RigthCap)      << 4 |
                   static_cast<uint8_t>(m_Join)          << 2 |
                   static_cast<uint8_t>(m_ColorToUse)
                );

                uint32_t data[] = {m_GeneralColor, thickness_antialias_border_miterlimit, m_BorderColor, caps_join_color};
                bgfx::setUniform(m_UniformData, data);
            }

    };
}