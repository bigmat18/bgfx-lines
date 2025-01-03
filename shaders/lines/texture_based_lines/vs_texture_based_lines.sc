$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../lines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;

uniform vec4 u_IndirectData;
#define maxTextureSize u_IndirectData.x

#define uv                    a_position
#define u_screenWidth         u_data1.x
#define u_screenHeigth        u_data1.y
#define u_thickness           u_data1.z

#define u_leftCap             u_data1.w
#define u_rigthCap            u_data2.x

IMAGE2D_RO(textureBuffer, rgba32f, 0);

void main() {
    uint p0_Y = (gl_InstanceID * 3) / maxTextureSize;
    uint p0_X = (gl_InstanceID * 3) - (p0_Y * maxTextureSize);
    
    uint p1_Y = ((gl_InstanceID * 3) + 1) / maxTextureSize;
    uint p1_X = ((gl_InstanceID * 3) + 1) - (p1_Y * maxTextureSize);

    uint color_Y = ((gl_InstanceID * 3) + 2) / maxTextureSize;
    uint color_X = ((gl_InstanceID * 3) + 2) - (color_Y * maxTextureSize);

    vec4 p0    = imageLoad(textureBuffer, ivec2(p0_X, p0_Y));
    vec4 p1    = imageLoad(textureBuffer, ivec2(p1_X, p1_Y));
    vec4 color = imageLoad(textureBuffer, ivec2(color_X, color_Y));

    vec4 p0_px = calculatePointWithMVP(p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(p1, u_screenWidth, u_screenHeigth);
    
    v_color = color;
    v_length = length(p1_px - p0_px);
    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_screenWidth, u_screenHeigth);
}