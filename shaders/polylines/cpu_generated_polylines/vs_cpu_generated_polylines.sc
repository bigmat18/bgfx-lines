$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_color

#include "../../polylines.sh"

uniform vec4 u_data;
uniform vec4 u_color;

#define a_prev            a_position
#define a_curr            a_texcoord0
#define a_next            a_texcoord1
#define a_uv              a_texcoord2 

#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_miter_limit     u_data.z
#define u_thickness       u_data.w

void main() { 
    v_color = u_color;
    gl_Position = calculatePolylines(a_prev, a_curr, a_next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
