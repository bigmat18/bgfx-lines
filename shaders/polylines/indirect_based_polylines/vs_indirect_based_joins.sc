$input a_position, i_data0, i_data1, i_data2
$output v_color

#include <bgfx_compute.sh>
#include "../../polylines.sh"

BUFFER_RO(pointsBuffer, float, 1);
#define p(pos)    vec4(pointsBuffer[0 + ((pos) * 3)], pointsBuffer[1 + ((pos) * 3)], pointsBuffer[2 + ((pos) * 3)], 0.0)

uniform vec4 u_data;
uniform vec4 u_color;
uniform vec4 u_IndirectData;

#define a_uv              a_position 
#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_miter_limit     u_data.z
#define u_thickness       u_data.w

void main() {
    uint index = gl_InstanceID + 1;
    vec4 prev = p(index - 1);
    vec4 curr = p(index);
    vec4 next = p(index + 1);

    v_color = u_color;
    gl_Position = calculatePolylines(prev, curr, next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
