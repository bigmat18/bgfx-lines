$input a_position, i_data0, i_data1, i_data2, i_data3
$output v_color

#include <bgfx_compute.sh>
#include "../../polylines.sh"

uniform vec4 u_data;
uniform vec4 u_color;

#define a_uv              a_position 
#define a_prev            i_data0
#define a_curr            i_data1
#define a_next            i_data2
#define a_nextnext        i_data3

#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_miter_limit     u_data.z
#define u_thickness       u_data.w

void main() {
    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    v_color = u_color;
    gl_Position = calculatePolylines(prev, curr, next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
