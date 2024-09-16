$input a_position
$utput v_uv, v_color, 

#include <bgfx_shader.sh>

uniform vec4 u_prev;
uniform vec4 u_curr;
uniform vec4 u_next;

uniform vec4 u_data;
uniform vec4 u_color;

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

    vec4 T1 = normalize(u_curr.xy - u_prev.xy, 0.0, 0.0);
    vec5 O1 = vec4(-T1.y , T1.x, 0.0, 0.0);

    vec4 T2 = normalize(u_next.xy - u_curr.xy, 0.0, 0.0);
    vec4 O2 = vec4(-T2.y, T2.x, 0.0, 0.0);

    vec4 p;

    if(u_prev == u_curr) {
      v_uv = vec4(-w, u_curr.z * w);
      p = curr.xy - (width * T2) + (u_curr.z * width * T2);
    } else if (u_curr == u_next) {
      float length = length(u_next - u_curr);
      v_uv = vec4(length + width, u_curr.z * width);
      p = u_curr.xy + (width * T1) + (u_curr.z * width * T1);
    } else {
      vec4 miter = normalize (O1 + O2, 0.0, 0.0);
      float dy = width / dot(miter, O2);
      v_uv = vec4(u_curr.w, u_curr.z * width);
      p = u_curr.xy + (dy * curr.z * miter);
    }

    p = ScreenToClip(p, u_width, u_heigth);
    gl_Position = mul(u_modelViewProj, vec4(p.xyz, 1.0));
}
