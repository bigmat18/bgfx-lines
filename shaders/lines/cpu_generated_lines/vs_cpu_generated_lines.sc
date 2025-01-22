$input a_position, a_texcoord0, a_color0, a_normal, a_texcoord1
$output v_color, v_uv, v_length, v_normal

#include <bgfx_shader.sh>
#include "../../lines.sh"

uniform vec4 u_data;

#define p0                    a_position
#define p1                    a_texcoord0
#define uv                    vec2(float(a_texcoord1.x), float(a_texcoord1.y))

void main() {
    vec4 u_general_color = uintToVec4FloatColor(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    float u_leftCap      = float((thickness_antialias_border_caps >> uint(4))  & uint(0x3));
    float u_rigthCap     = float((thickness_antialias_border_caps >> uint(2))  & uint(0x3));
    float u_color_to_use = float((thickness_antialias_border_caps >> uint(0))  & uint(0x3));


    // ======================================

    vec4 NDC_p0 = mul(u_modelViewProj, vec4(p0.xyz, 1.0));
    vec4 NDC_p1 = mul(u_modelViewProj, vec4(p1.xyz, 1.0));

    vec4 screen_p0 = vec4(((NDC_p0.xy / NDC_p0.w)).xy, 0.0, 0.0);
    vec4 screen_p1 = vec4(((NDC_p1.xy / NDC_p1.w)).xy, 0.0, 0.0);

    vec4 p0_px = clipToScreen(screen_p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = clipToScreen(screen_p1, u_screenWidth, u_screenHeigth);
    
    // v_color = (a_color0 * (1 - sign(u_color_to_use))) + (u_general_color * sign(u_color_to_use));
    v_normal = a_normal;
    v_length = length(p1_px - p0_px);

    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);

    vec4 T = normalize(p1_px - p0_px);
    vec4 N = vec4(-T.y, T.x, 0.0, 0.0);
    
    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    float width_px = (u_thickness / 2) + u_antialias + u_border;
    float activeCaps = sign((u_leftCap * (1 - uv.x)) + (u_rigthCap * uv.x));
    vec4 p = p0_px + (uv.x * T * v_length) + (v * N * width_px) + (u * T * ((activeCaps * (u_thickness/2)) + u_antialias + u_border)); 
    p = screenToClip(p, u_screenWidth, u_screenHeigth);

    float z = ((1 - uv.x) * (NDC_p0.z)) + (uv.x * (NDC_p1.z));
	float w = ((1 - uv.x) * (NDC_p0.w)) + (uv.x * (NDC_p1.w));
    p = vec4(p.xy * w, z, w); 
    v_color = vec4(p.z, 0, 0, 1);
    gl_Position = p;

}
