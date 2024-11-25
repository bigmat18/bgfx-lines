$input a_position, i_data0, i_data1, i_data2
$output v_color

#include <bgfx_compute.sh>
#include "../../polylines.sh"

uniform vec4 u_data;
uniform vec4 u_color;

#define a_uv              a_position 
#define a_prev            i_data0
#define a_curr            i_data1
#define a_next            i_data2

#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_miter_limit     u_data.z
#define u_thickness       u_data.w

void main() {
    v_color = u_color;
    gl_Position = calculatePolylines(a_prev, a_curr, a_next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
