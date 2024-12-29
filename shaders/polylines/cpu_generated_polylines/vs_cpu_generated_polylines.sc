$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_color, v_uv, v_length

#include "../../polylines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_color;

#define a_prev            a_position
#define a_curr            a_texcoord0
#define a_next            a_texcoord1
#define a_uv              a_texcoord2

#define u_screenWidth           u_data1.x
#define u_screenHeigth          u_data1.y
#define u_miter_limit           u_data1.z
#define u_thickness             u_data1.w

#define u_leftCap             u_data2.x
#define u_rigthCap            u_data2.y

void main() { 
    vec4 prev_px = calculatePointWithMVP(a_prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(a_curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(a_next, u_screenWidth, u_screenHeigth);

    v_color = u_color;
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev_px, curr_px, next_px, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap);
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth);
}
