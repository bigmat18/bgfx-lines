$input a_position, i_data0, i_data1, i_data2
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
    uint index = gl_InstanceID + 1;

    vec4 prev = imageLoad(textureBuffer, calculateTextureCoord(index - 1));
    vec4 curr = imageLoad(textureBuffer, calculateTextureCoord(index));
    vec4 next = imageLoad(textureBuffer, calculateTextureCoord(index + 1));

    v_color = u_color;
    gl_Position = calculatePolylines(prev, curr, next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
