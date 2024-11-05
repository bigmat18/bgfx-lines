$input a_position, a_texcoord0, a_texcoord1
$output v_color

#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_color;

#define p0                    a_position
#define p1                    a_texcoord0
#define uv                    a_texcoord1

#define u_screenWidth         u_data.x
#define u_screenHeigth        u_data.y
#define u_antialias           u_data.z
#define u_thickness           u_data.w

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
    vec4 NDC_p0 = mul(u_modelViewProj, vec4(p0.xyz, 1.0));
    vec4 NDC_p1 = mul(u_modelViewProj, vec4(p1.xyz, 1.0));

    vec4 screen_p0 = vec4(((NDC_p0.xy / NDC_p0.w)).xy, 0.0, 0.0);
    vec4 screen_p1 = vec4(((NDC_p1.xy / NDC_p1.w)).xy, 0.0, 0.0); 

    vec4 p0_px = ClipToScreen(screen_p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = ClipToScreen(screen_p1, u_screenWidth, u_screenHeigth); 

    float width_px = u_thickness / 2.0 + u_antialias;
    float length_px = length(p1_px - p0_px);
    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    vec4 T = normalize(p1_px - p0_px);
    vec4 N = vec4(-T.y, T.x, 0.0, 0.0);

    vec4 p = p0_px + (uv.x * T * length_px) + (u * T * width_px) + (v * N * width_px); 
    p = ScreenToClip(p, u_screenWidth, u_screenHeigth);

    v_color = u_color;
    gl_Position = vec4(p.xy, NDC_p0.z / NDC_p0.w, 1.0);
}
