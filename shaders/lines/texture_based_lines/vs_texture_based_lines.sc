$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../lines.sh"

uniform vec4 u_data;
uniform vec4 u_IndirectData;

#define maxTextureSize        u_IndirectData.x
#define uv                    a_position

IMAGE2D_RO(textureBuffer, rgba32f, 0);

void main() {
    uint screenSize = floatBitsToUint(u_data.x);
    uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
    
    float u_screenWidth  = float((screenSize >> uint(16)) & uint(0xFFFF));
    float u_screenHeigth = float(screenSize & uint(0xFFFF));

    float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    float u_leftCap      = float((thickness_antialias_border_caps >> uint(2))  & uint(0x2));
    float u_rigthCap     = float(thickness_antialias_border_caps               & uint(0x2));

    uint p0_Y = (gl_InstanceID * 4) / maxTextureSize;
    uint p0_X = (gl_InstanceID * 4) - (p0_Y * maxTextureSize);
    
    uint p1_Y = ((gl_InstanceID * 4) + 1) / maxTextureSize;
    uint p1_X = ((gl_InstanceID * 4) + 1) - (p1_Y * maxTextureSize);

    uint color0_Y = ((gl_InstanceID * 4) + 2) / maxTextureSize;
    uint color0_X = ((gl_InstanceID * 4) + 2) - (color0_Y * maxTextureSize);

    uint color1_Y = ((gl_InstanceID * 4) + 3) / maxTextureSize;
    uint color1_X = ((gl_InstanceID * 4) + 3) - (color1_Y * maxTextureSize);

    vec4 p0     = imageLoad(textureBuffer, ivec2(p0_X, p0_Y));
    vec4 p1     = imageLoad(textureBuffer, ivec2(p1_X, p1_Y));
    vec4 color0 = imageLoad(textureBuffer, ivec2(color0_X, color0_Y));
    vec4 color1 = imageLoad(textureBuffer, ivec2(color1_X, color1_Y));

    vec4 p0_px = calculatePointWithMVP(p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(p1, u_screenWidth, u_screenHeigth);
    
    v_color = (color0 * (1 - uv.x)) + (color1 * uv.x);
    v_length = length(p1_px - p0_px);
    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);
}