$input a_position, a_texcoord0, a_texcoord1
$output v_color

#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_color;

#define a_p0         a_position
#define a_p1         a_texcoord0
#define a_uv         a_texcoord1

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

    vec4 NDC_p0 = mul(u_modelViewProj, vec4(a_p0.xyz, 1.0));
    vec4 NDC_p1 = mul(u_modelViewProj, vec4(a_p1.xyz, 1.0));

    vec4 screen_p0 = vec4(((NDC_p0.xy / NDC_p0.w)).xy, 0.0, 0.0);
    vec4 screen_p1 = vec4(((NDC_p1.xy / NDC_p1.w)).xy, 0.0, 0.0);

    vec4 p0 = ClipToScreen(screen_p0, u_screenWidth, u_screenHeigth);
    vec4 p1 = ClipToScreen(screen_p1, u_screenWidth, u_screenHeigth);

    float line_width = u_thickness / 2.0 + u_antialias;
    vec4 length = ClipToScreen(vec4(length(p1-p0), length(p1-p0), 0, 0), u_screenWidth, u_screenHeigth);

    vec4 T0 = vec4(normalize(p1.xy - p0.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    float u = 2.0 * a_uv.x - 1.0;
    float v = 2.0 * a_uv.y - 1.0;

    vec4 p = p0 + (a_uv.x * T0 * length) + (u * T0 * line_width) + (v * N0 * line_width);

    v_color = u_color;
    p = ScreenToClip(p, u_screenWidth, u_screenHeigth);
    gl_Position = vec4(p.xy, NDC_p0.z / NDC_p0.w, 1.0);

    v_color = vec4(1.0, 0.0, 0.0, 1.0);
    gl_Position = mul(u_modelViewProj,vec4(a_uv.x, a_uv.y, 0.0, 1.0));
}
