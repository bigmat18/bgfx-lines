$input a_position, a_texcoord0, a_color0, a_texcoord1
$output v_color, v_uv, v_length

#include <bgfx_shader.sh>
#include "../../utils.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;

#define p0                    a_position
#define p1                    a_texcoord0
#define uv                    a_texcoord1

#define u_screenWidth         u_data1.x
#define u_screenHeigth        u_data1.y
#define u_thickness           u_data1.z

#define u_leftCap             u_data1.w
#define u_rigthCap            u_data2.x

void main() {
    vec4 NDC_p0 = mul(u_modelViewProj, vec4(p0.xyz, 1.0));
    vec4 NDC_p1 = mul(u_modelViewProj, vec4(p1.xyz, 1.0));

    vec4 screen_p0 = vec4(((NDC_p0.xy / NDC_p0.w)).xy, 0.0, 0.0);
    vec4 screen_p1 = vec4(((NDC_p1.xy / NDC_p1.w)).xy, 0.0, 0.0); 

    vec4 p0_px = clipToScreen(screen_p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = clipToScreen(screen_p1, u_screenWidth, u_screenHeigth); 

    float width_px = u_thickness / 2.0;
    float length_px = length(p1_px - p0_px);
    
    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    vec4 T = normalize(p1_px - p0_px);
    vec4 N = vec4(-T.y, T.x, 0.0, 0.0);

    vec4 p = p0_px + (uv.x * T * length_px) + (v * N * width_px);
    p = screenToClip(p, u_screenWidth, u_screenHeigth);

    T = vec4(1.0, 0.0, 0.0, 0.0);
    N = vec4(0.0, 1.0, 0.0, 0.0);
    float activeCaps = sign((u_leftCap * (1 - uv.x)) + (u_rigthCap * uv.x));
    v_uv = (uv.x * length_px * T) + (u * T * width_px * activeCaps) + (v * N * width_px);
    v_length = length_px;

    v_color = a_color0;
    gl_Position = vec4(p.xy, NDC_p0.z / NDC_p0.w, 1.0);
}
