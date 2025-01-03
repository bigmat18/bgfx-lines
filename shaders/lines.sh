#ifndef POLYLINES
#define POLYLINES 

#include "utils.sh"

vec4 calculateLines(vec4 p0_px, vec4 p1_px, vec2 uv, float length_px, float thickness, float screen_width, float screen_heigth) {
    float width_px = thickness / 2.0;
    
    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    vec4 T = normalize(p1_px - p0_px);
    vec4 N = vec4(-T.y, T.x, 0.0, 0.0);

    vec4 p = p0_px + (uv.x * T * length_px) + (v * N * width_px); 
    p = screenToClip(p, screen_width, screen_heigth);

    float z = ((1 - uv.x) * (p0_px.z / p0_px.w)) + (uv.x * (p1_px.z / p1_px.w));
    return vec4(p.xy, z, 1.0); 
}

vec4 calculateLinesUV(vec4 p0_px, vec4 p1_px, vec2 uv, float length_px, float thickness, float leftCap, float rigthCap) {
    vec4 T = vec4(1.0, 0.0, 0.0, 0.0);
    vec4 N = vec4(0.0, 1.0, 0.0, 0.0);

    float width_px = thickness / 2.0;

    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    float activeCaps = sign((leftCap * (1 - uv.x)) + (rigthCap * uv.x));
    return (uv.x * length_px * T) + (u * T * width_px * activeCaps) + (v * N * width_px);
}

#endif