$input a_position, i_data0, i_data1, i_data2, i_data3
$output v_color

#include <bgfx_shader.sh>

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

#define EPSILON           0.1

vec4 ScreenToClip(vec4 coordinate, float width, float heigth) {
    return vec4(
                  (2 * coordinate.x / width), 
                  (2 * coordinate.y / heigth), 
                  coordinate.z, 
                  coordinate.w
                );
}

vec4 ClipToScreen(vec4 coordinate, float width, float heigth) {
    return vec4(
                  (coordinate.x * width) / 2, 
                  (coordinate.y * heigth) / 2, 
                  coordinate.z, 
                  coordinate.w
                );
}

void main() {
    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    vec4 NDC_prev = mul(u_modelViewProj, vec4(prev.xyz, 1.0));
    vec4 NDC_curr = mul(u_modelViewProj, vec4(curr.xyz, 1.0));
    vec4 NDC_next = mul(u_modelViewProj, vec4(next.xyz, 1.0));

    vec4 screen_prev = vec4(((NDC_prev.xy / NDC_prev.w)).xy, 0.0, 0.0);
    vec4 screen_curr = vec4(((NDC_curr.xy / NDC_curr.w)).xy, 0.0, 0.0);   
    vec4 screen_next = vec4(((NDC_next.xy / NDC_next.w)).xy, 0.0, 0.0);

    prev = ClipToScreen(screen_prev, u_width, u_heigth);
    curr = ClipToScreen(screen_curr, u_width, u_heigth);
    next = ClipToScreen(screen_next, u_width, u_heigth);

    float half_thickness = u_thickness / 2.0;

    vec4 T0 = vec4(normalize(curr.xy - prev.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    vec4 T1 = vec4(normalize(next.xy - curr.xy).xy, 0.0, 0.0);
    vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    vec4 p;

    float u = 2.0 * a_uv.x - 1.0;
    float v = 2.0 * a_uv.y - 1.0;

    if(curr.x == prev.x && curr.y == prev.y) {
    
      p = curr + (v * half_thickness * N1);

    } else if (curr.x == next.x && curr.y == next.y) {

      p = curr + (v * half_thickness * N0);

    } else {

      vec4 miter_direction = normalize(N0 + N1);
      float miter_length = max(half_thickness / max(dot(miter_direction, N1), EPSILON), half_thickness);
      
      vec4 miter = miter_direction * miter_length;
      float total_width = length(miter) * 2;

      if(total_width > u_miter_limit) {
          vec4 new_miter_direction = (N1 * (1-a_uv.x)) + (N0 * a_uv.x);
          miter = new_miter_direction * half_thickness;
      }

      p = curr + (v * miter);
    } 

    v_color = u_color;
    p = ScreenToClip(p, u_width, u_heigth);
    gl_Position = vec4(p.xy, NDC_curr.z / NDC_curr.w, 1.0);
}
