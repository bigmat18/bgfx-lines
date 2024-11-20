$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_color

#include <bgfx_shader.sh>

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
    v_color = u_color;

    vec4 NDC_prev = mul(u_modelViewProj, vec4(a_prev.xyz, 1.0));
    vec4 NDC_curr = mul(u_modelViewProj, vec4(a_curr.xyz, 1.0));
    vec4 NDC_next = mul(u_modelViewProj, vec4(a_next.xyz, 1.0));

    vec4 screen_prev = vec4(((NDC_prev.xy / NDC_prev.w)).xy, 0.0, 0.0);
    vec4 screen_curr = vec4(((NDC_curr.xy / NDC_curr.w)).xy, 0.0, 0.0);   
    vec4 screen_next = vec4(((NDC_next.xy / NDC_next.w)).xy, 0.0, 0.0);

    vec4 prev = ClipToScreen(screen_prev, u_width, u_heigth);
    vec4 curr = ClipToScreen(screen_curr, u_width, u_heigth);
    vec4 next = ClipToScreen(screen_next, u_width, u_heigth);

    float line_width = u_thickness / 2.0;

    vec4 T0 = vec4(normalize(curr.xy - prev.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    vec4 T1 = vec4(normalize(next.xy - curr.xy).xy, 0.0, 0.0);
    vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    vec4 p;

    float u = 2.0 * a_uv.x - 1.0;
    float v = 2.0 * a_uv.y - 1.0;

    if(a_prev.x == a_curr.x && a_prev.y == a_curr.y) {
    
      p = curr - (line_width * T1) + (v * line_width * N1);

    } else if (a_curr.x == a_next.x && a_curr.y == a_next.y) {

      p = curr + (line_width * T0) + (v * line_width * N0);

    } else {

      vec4 miter = normalize(N0 + N1);
      float cos_theta = dot(miter, N1);

      float miter_length = line_width / max(cos_theta, EPSILON);
      float actual_miter_length = max(miter_length, line_width);
      
      vec4 final_direction = miter * actual_miter_length;
      float total_width = length(final_direction) * 2;

      if(total_width > u_miter_limit) {
          vec4 new_miter = (N1 * (1-a_uv.x)) + (N0 * a_uv.x);
          final_direction = new_miter * line_width;
      }
      p = curr + (v * final_direction);

    } 

    p = ScreenToClip(p, u_width, u_heigth);
    gl_Position = vec4(p.xy, NDC_curr.z / NDC_curr.w, 1.0);
}
