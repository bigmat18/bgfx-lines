$input a_position, a_texcoord0, a_texcoord1
$output v_uv, v_color

#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_color;
uniform vec4 u_length;

#define a_prev a_position
#define a_curr a_texcoord0
#define a_next a_texcoord1

#define u_linelength    u_length.x
#define u_width         u_data.x
#define u_heigth        u_data.y
#define u_antialias     u_data.z
#define u_thickness     u_data.w

vec4 ScreenToClip(vec4 coordinate, float width, float height) {
    return vec4((2.0 * coordinate.x/ width) - 1, 
                (2.0 * coordinate.y/ height) - 1, 
                coordinate.z, 
                coordinate.w);
} 

void main() {  
    v_color = u_color;  
    float width = u_thickness / 2.0 + u_antialias;

    vec4 T0 = vec4(normalize(a_curr.xy - a_prev.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    vec4 T1 = vec4(normalize(a_next.xy - a_curr.xy).xy, 0.0, 0.0);
    vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    vec4 p;

    if(a_prev.x == a_curr.x && a_prev.y == a_curr.y) {
      
      v_uv = vec4(-width, a_curr.z * width, 0.0, 0.0);
      p = vec4(a_curr.xy, 0.0, 0.0) - (width * T1) + (a_curr.z * width * N1);

    } else if (a_curr.x == a_next.x && a_curr.y == a_next.y) { 

      v_uv = vec4(u_linelength + width, a_curr.z * width, 0.0, 0.0);
      p = vec4(a_curr.xy, 0.0, 0.0) + (width * T0) + (a_curr.z * width * N0);

    } else {
      
      vec4 miter = normalize(N0 + N1);
      float dy = width / dot(miter, N1);

      v_uv = vec4(a_curr.w, a_curr.z * width, 0.0, 0.0);
      p = vec4(a_curr.xy, 0.0, 0.0) + (dy * a_curr.z * miter);

    }

    p = ScreenToClip(p, u_width, u_heigth);
    gl_Position = vec4(p.xy, 0.0, 1.0);
}
