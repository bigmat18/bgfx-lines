$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_uv, v_color, v_prev, v_curr, v_next

#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_color;

#define a_prev            a_position
#define a_curr            a_texcoord0
#define a_next            a_texcoord1
#define a_uv              a_texcoord2 

#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_linelength      u_data.z
#define u_thickness       u_data.w

#define EPSILON           0.2

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
    v_next = a_next;
    v_curr = a_curr;
    v_prev = a_prev;

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
    v_uv = vec4(a_uv.y, 2 * (a_uv.x * line_width) / u_heigth, 0.0, 0.0);


    if(a_prev.x == a_curr.x && a_prev.y == a_curr.y) {
    
      p = curr - (line_width * T1) + (a_uv.x * line_width * N1);

    } else if (a_curr.x == a_next.x && a_curr.y == a_next.y) {

      p = curr + (line_width * T0) + (a_uv.x  * line_width * N0);

    } else {
      vec4 bisector_simple = vec4(N0.xy, 0.0, 0.0);
      vec4 bisector = normalize(N0 + N1);
      float cos_theta = dot(bisector, N1);

      float min_threshold = 0.1;
      float max_threshold = 0.3;
      float t = smoothstep(min_threshold, max_threshold, abs(cos_theta));


      float miter_length = line_width / max(cos_theta, EPSILON);
      float actual_miter_length = max(miter_length, line_width);

      vec4 smooth_bisector = mix(bisector_simple, bisector, t);
      float miter_length_smooth = mix(line_width, actual_miter_length, t);

      p = curr + (actual_miter_length * a_uv.x * bisector);

    } 

    p = ScreenToClip(p, u_width, u_heigth);
    v_color = u_color;
    gl_Position = vec4(p.xy, NDC_curr.z / NDC_curr.w, 1.0);
    // gl_Position = vec4(v_uv.x, v_uv.y, v_uv.z, 1.0);
}
