$input a_position
$output v_color

#include <bgfx_compute.sh>
#include "../../polylines.sh"

IMAGE2D_RO(textureBuffer, rgba32f, 0);

uniform vec4 u_data;
uniform vec4 u_color;
uniform vec4 u_IndirectData;

#define maxInstancingNum u_IndirectData.x
#define maxTextureSize u_IndirectData.y

#define a_uv              a_position 
#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_miter_limit     u_data.z
#define u_thickness       u_data.w

ivec2 calculateTextureCoord(uint index) {
    uint Y = index / maxTextureSize;
    uint X = index - (Y * maxTextureSize);
    return ivec2(X, Y);
}

void main() {
    vec4 a_prev = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID - sign(gl_InstanceID)));
    vec4 a_curr = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID));
    vec4 a_next = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID + sign(maxInstancingNum - gl_InstanceID)));
    vec4 a_nextnext = imageLoad(textureBuffer, calculateTextureCoord(gl_InstanceID + sign(maxInstancingNum - gl_InstanceID) + sign(maxInstancingNum - 1 - gl_InstanceID)));

    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    v_color = u_color;
    gl_Position = calculatePolylines(prev, curr, next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
