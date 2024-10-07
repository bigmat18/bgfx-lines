$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_uv, v_normal, v_thickness, v_color

#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_color;
uniform vec4 u_length;

#define a_prev          a_position
#define a_curr          a_texcoord0
#define a_next          a_texcoord1
#define a_uv            a_texcoord2 

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

    //vec4 prev = ScreenToClip(a_prev, u_width, u_heigth);
    //vec4 curr = ScreenToClip(a_curr, u_width, u_heigth);
    //vec4 next = ScreenToClip(a_next, u_width, u_heigth);

    vec4 NDC_prev = u_modelViewProj * vec4(a_prev.xyz, 1.0);
    vec4 NDC_curr = u_modelViewProj * vec4(a_curr.xyz, 1.0);
    vec4 NDC_next = u_modelViewProj * vec4(a_next.xyz, 1.0);

    vec4 screen_prev = vec4(u_width * ((NDC_prev.x / NDC_prev.w) + 1.0) / 2.0,
                            u_heigth * ((NDC_prev.y / NDC_prev.w) + 1.0) / 2.0,
                            0, 0);

    vec4 screen_curr = vec4(u_width * ((NDC_curr.x / NDC_curr.w) + 1.0) / 2.0,
                            u_heigth * ((NDC_curr.y / NDC_curr.w) + 1.0) / 2.0,
                            0, 0);
                          
    vec4 screen_next = vec4(u_width * ((NDC_next.x / NDC_next.w) + 1.0) / 2.0,
                            u_heigth * ((NDC_next.y / NDC_next.w) + 1.0) / 2.0,
                            0, 0);

/*
    vec4 normal = u_model * vec4(a_curr.xyz, 1.0);
    v_normal = normal;
    if(normal.z < 0) 
      v_thickness = vec4(u_thickness / 2.0, 0.0, 0.0, 0.0);
    else 
      v_thickness = vec4(u_thickness*(pow(normal.z, 0.5) + 1 ) / 2.0, 0.0, 0.0, 0.0);
*/

    float width = u_thickness / 2.0 + u_antialias;

    vec4 T0 = vec4(normalize(screen_curr.xy - screen_prev.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    vec4 T1 = vec4(normalize(screen_next.xy - screen_curr.xy).xy, 0.0, 0.0);
    vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    vec4 p;
    v_uv = vec4(a_uv.y, a_uv.x * width, 0.0, 0.0);

    if(a_prev.x == a_curr.x && a_prev.y == a_curr.y) {
      
      v_uv.x = -width;
      p = screen_curr - (width * T1) + (a_uv.x * width * N1);

    } else if (a_curr.x == a_next.x && a_curr.y == a_next.y) { 

      v_uv.x = u_linelength + width;
      p = screen_curr + (width * T0) + (a_uv.x  * width * N0);

    } else {
      
      vec4 miter = normalize(N0 + N1);
      float dy = width / max(dot(miter, N1), 1.0);
      p = screen_curr + (dy * a_uv.x * miter);    
    } 

    v_color = u_color;
    p = ScreenToClip(p, u_width, u_heigth);
    gl_Position = vec4(p.xy, NDC_curr.z / NDC_curr.w, 1.0);
}
