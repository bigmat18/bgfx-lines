#pragma once
#include <iostream>
#include <assert.h>
#include <bgfx/bgfx.h>
#include "utils.hpp"

namespace lines {

    class LinesSettings {
        private:
            uint8_t m_Thickness;
            uint8_t m_Antialias;
            uint8_t m_Border;
            uint16_t m_ScreenWidth;
            uint16_t m_ScreenHeigth;
            uint32_t m_BorderColor;
            uint8_t m_MiterLimit;

            Caps m_LeftCap;
            Caps m_RigthCap;
            Joins m_Join;

            bgfx::UniformHandle m_UniformData;

        public:
            LinesSettings() = default;

            LinesSettings(uint16_t width, uint16_t heigth) : 
                m_Thickness(5),
                m_Antialias(0),
                m_Border(0),
                m_ScreenWidth(width),
                m_ScreenHeigth(heigth),
                m_BorderColor(COLOR(0, 0, 0, 1)),
                m_MiterLimit(m_Thickness * 2),
                m_LeftCap(Caps::ROUND_CAP),
                m_RigthCap(Caps::ROUND_CAP),
                m_Join(Joins::ROUND_JOIN)
            {
                m_UniformData = bgfx::createUniform("u_data", bgfx::UniformType::Vec4);
            }

            void setThickness(uint8_t thickness) { m_Thickness = thickness; }

            void setAntialias(uint8_t antialias) { m_Antialias = antialias; }

            void setBorder(uint8_t border) { m_Border = border; }

            void setScreenSize(uint16_t width, uint16_t heigth) {
                m_ScreenWidth = width;
                m_ScreenHeigth = heigth;
            }

            void setBorderColor(uint32_t borderColor) { m_BorderColor = borderColor; }

            void setMiterLimit(uint8_t miterLimit) {
                if(miterLimit < m_Thickness)
                    assert((void("Miter limit must be greatest then thickness * 2"), false));
                m_MiterLimit = miterLimit;
            }

            void setLeftCap(Caps cap) { m_LeftCap = cap; }

            void setRigthCap(Caps cap) { m_RigthCap = cap; }

            void setJoin(Joins join) { m_Join = join; }

        public:
            void bindUniformLines() const {
                uint32_t screenSize = m_ScreenWidth << 16 | m_ScreenHeigth;
                uint32_t thickness_antialias_border_caps = (
                    m_Thickness                       << 24 |
                    m_Antialias                       << 16 |
                    m_Border                          << 8  |
                    static_cast<uint8_t>(m_LeftCap)   << 2  |
                    static_cast<uint8_t>(m_RigthCap)  << 0  
                );
                uint32_t data[] = {screenSize, thickness_antialias_border_caps, m_BorderColor, 0};
                bgfx::setUniform(m_UniformData, data);
            }

            void bindUniformPolylines() const {
                uint32_t screenSize = m_ScreenWidth << 16 | m_ScreenHeigth;
                uint32_t thickness_antialias_border_miterlimit = (
                    m_Thickness                       << 24 |
                    m_Antialias                       << 16 |
                    m_Border                          << 8  |
                    m_MiterLimit                      << 0 
                );

                uint32_t caps_join = (
                   static_cast<uint8_t>(m_LeftCap)   << 4  |
                   static_cast<uint8_t>(m_RigthCap)  << 2  |
                   static_cast<uint8_t>(m_Join)      << 0  
                );

                uint32_t data[] = {screenSize, thickness_antialias_border_miterlimit, m_BorderColor, caps_join};
                bgfx::setUniform(m_UniformData, data);
            }

    };
}