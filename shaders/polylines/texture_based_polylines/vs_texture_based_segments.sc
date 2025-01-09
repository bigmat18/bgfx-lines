$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../polylines.sh"

IMAGE2D_RO(textureBuffer, rgba32f, 0);

uniform vec4 u_data;
uniform vec4 u_IndirectData;

#define maxInstancingNum        u_IndirectData.x
#define maxTextureSize          u_IndirectData.y
#define a_uv                    a_position 

void main() {
    uint screenSize = floatBitsToUint(u_data.x);
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_join = floatBitsToUint(u_data.w);
    
    float u_screenWidth  = float((screenSize >> uint(16)) & uint(0xFFFF));
    float u_screenHeigth = float(screenSize & uint(0xFFFF));

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    float u_miter_limit  = float(thickness_antialias_border_miterlimit               & uint(0xFF));
    
    float u_leftCap      = float((caps_join >> uint(4))  & uint(0x3));
    float u_rigthCap     = float((caps_join >> uint(2))  & uint(0x3));
    float u_join         = float(caps_join               & uint(0x3));

    vec4 a_prev     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID - sign(gl_InstanceID)) * 2, maxTextureSize));
    vec4 a_curr     = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID * 2, maxTextureSize));
    vec4 a_next     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID + 1) * 2, maxTextureSize));
    vec4 a_nextnext = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID + 1 + sign(maxInstancingNum - 1 - gl_InstanceID)) * 2, maxTextureSize));

    vec4 color0 = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 2) + 1, maxTextureSize));
    vec4 color1 = imageLoad(textureBuffer, calculateTextureCoord(((gl_InstanceID + 1) * 2) + 1, maxTextureSize));

    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = (color0 * (1 - a_uv.x)) + (color1 * a_uv.x);
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev, curr, next, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap, u_join);
    
    bool is_start = curr.x == prev.x && curr.y == prev.y;
    bool is_end = curr.x == next.x && curr.y == next.y;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}

