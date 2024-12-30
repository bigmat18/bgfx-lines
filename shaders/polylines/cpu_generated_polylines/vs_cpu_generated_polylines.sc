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
    vec4 prev_px = calculatePointWithMVP(vec4(a_prev, 0.0), u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(vec4(a_curr, 0.0), u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(vec4(a_next, 0.0), u_screenWidth, u_screenHeigth);

    v_color = u_color;
    v_length = length(((next_px - curr_px) * (1 - a_uv.x)) + ((curr_px - prev_px) * (a_uv.x)));

    v_uv = calculatePolylinesUV(prev_px, curr_px, next_px, a_uv, u_thickness, v_length, u_leftCap, u_rigthCap);
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth);

    // float half_thickness = u_thickness / 2.0;

    // vec4 T0 = vec4(normalize(curr_px.xy - prev_px.xy).xy, 0.0, 0.0);
    // vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    // vec4 T1 = vec4(normalize(next_px.xy - curr_px.xy).xy, 0.0, 0.0);
    // vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    // vec4 p;

    // float u = 2.0 * a_uv.x - 1.0;
    // float v = 2.0 * a_uv.y - 1.0;

    // if(curr_px.x == prev_px.x && curr_px.y == prev_px.y) {
    
    //   p = curr_px + (v * half_thickness * N1);
    //   v_color = vec4(0.0, 1.0, 1.0, 1.0);  

    // } else if (curr_px.x == next_px.x && curr_px.y == next_px.y) {

    //   p = curr_px + (v * half_thickness * N0);
    //   v_color = vec4(0.0, 1.0, 0.0, 1.0);

    // } else {

    //     vec4 miter_direction = normalize(N0 + N1);
    //     float cos_theta = dot(miter_direction, N1);
    //     float miter_length = max(half_thickness / max(cos_theta, 0.1), half_thickness);
      
    //     vec4 miter = miter_direction * miter_length;
    //     float total_width = length(miter) * 2;

    //     vec4 new_miter_direction = (N1 * (1-a_uv.x)) + (N0 * a_uv.x);
    //     vec4 miter_plane = new_miter_direction * half_thickness;

    //     float min_threshold = u_miter_limit;
    //     float max_threshold = u_miter_limit * 5;
    //     float t = smoothstep(min_threshold, max_threshold, total_width);

    //     vec4 final_miter = mix(miter, miter_plane, t);

    //     #if 0
    //       if(total_width > u_miter_limit) {
    //           vec4 new_miter_direction = (N1 * (1-a_uv.x)) + (N0 * a_uv.x);
    //           miter = new_miter_direction * half_thickness;
    //       }
    //     #endif

    //     p = curr_px + (v * final_miter);
    // } 

    // p = screenToClip(p, u_screenWidth, u_screenHeigth);
    // gl_Position = vec4(p.xy, curr_px.z / curr_px.w, 1.0);
}
