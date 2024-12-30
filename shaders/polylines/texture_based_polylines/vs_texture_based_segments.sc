$input a_position
$output v_color, v_uv, v_length

#include <bgfx_compute.sh>
#include "../../polylines.sh"

IMAGE2D_RO(textureBuffer, rgba32f, 0);

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_color;
uniform vec4 u_IndirectData;

#define maxInstancingNum        u_IndirectData.x
#define maxTextureSize          u_IndirectData.y

#define a_uv                    a_position 
#define u_screenWidth           u_data1.x
#define u_screenHeigth          u_data1.y
#define u_miter_limit           u_data1.z
#define u_thickness             u_data1.w

#define u_leftCap               u_data2.x
#define u_rigthCap              u_data2.y

ivec2 calculateTextureCoord(uint index) {
    uint Y = index / maxTextureSize;
    uint X = index - (Y * maxTextureSize);
    return ivec2(X, Y);
}

void main() {
    vec4 a_prev = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID - sign(gl_InstanceID)));
    vec4 a_curr = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID));
    vec4 a_next = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID + 1));
    vec4 a_nextnext = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID + 1 + sign(maxInstancingNum - 1 - gl_InstanceID)));

    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = u_color;
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev_px, curr_px, next_px, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap);
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth);
}
