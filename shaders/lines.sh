#ifndef POLYLINES
#define POLYLINES 

#include "utils.sh"

vec4 calculateLines(vec4 p0, vec4 p1, vec2 uv, float thickness, float screen_width, float screen_heigth) {
    vec4 NDC_p0 = mul(u_modelViewProj, vec4(p0.xyz, 1.0));
    vec4 NDC_p1 = mul(u_modelViewProj, vec4(p1.xyz, 1.0));

    vec4 screen_p0 = vec4(((NDC_p0.xy / NDC_p0.w)).xy, 0.0, 0.0);
    vec4 screen_p1 = vec4(((NDC_p1.xy / NDC_p1.w)).xy, 0.0, 0.0); 

    vec4 p0_px = clipToScreen(screen_p0, screen_width, screen_heigth);
    vec4 p1_px = clipToScreen(screen_p1, screen_width, screen_heigth); 

    float width_px = thickness / 2.0;
    float length_px = length(p1_px - p0_px);
    
    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    vec4 T = normalize(p1_px - p0_px);
    vec4 N = vec4(-T.y, T.x, 0.0, 0.0);

    vec4 p = p0_px + (uv.x * T * length_px) + (v * N * width_px); 
    p = screenToClip(p, screen_width, screen_heigth);

    float z = ((1 - uv.x) * (NDC_p0.z / NDC_p0.w)) + (uv.x * (NDC_p1.z / NDC_p1.w));
    return vec4(p.xy, z, 1.0); 
}

#endif