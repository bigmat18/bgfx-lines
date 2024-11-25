#ifndef POLYLINES
#define POLYLINES

#include <bgfx_shader.sh>
#include "utils.sh"

vec4 calculatePolylines(vec4 prev, vec4 curr, vec4 next, vec2 uv, float thickness, float miter_limit, float screen_width, float screen_heigth) {
    vec4 NDC_prev = mul(u_modelViewProj, vec4(prev.xyz, 1.0));
    vec4 NDC_curr = mul(u_modelViewProj, vec4(curr.xyz, 1.0));
    vec4 NDC_next = mul(u_modelViewProj, vec4(next.xyz, 1.0));

    vec4 screen_prev = vec4((NDC_prev.xy / NDC_prev.w).xy, 0.0, 0.0);
    vec4 screen_curr = vec4((NDC_curr.xy / NDC_curr.w).xy, 0.0, 0.0);
    vec4 screen_next = vec4((NDC_next.xy / NDC_next.w).xy, 0.0, 0.0);

    prev = clipToScreen(screen_prev, screen_width, screen_heigth);
    curr = clipToScreen(screen_curr, screen_width, screen_heigth);
    next = clipToScreen(screen_next, screen_width, screen_heigth);

    float half_thickness = thickness / 2.0;

    vec4 T0 = vec4(normalize(curr.xy - prev.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    vec4 T1 = vec4(normalize(next.xy - curr.xy).xy, 0.0, 0.0);
    vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    vec4 p;

    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    if(curr.x == prev.x && curr.y == prev.y) {
    
      p = curr + (v * half_thickness * N1);

    } else if (curr.x == next.x && curr.y == next.y) {

      p = curr + (v * half_thickness * N0);

    } else {

      vec4 miter_direction = normalize(N0 + N1);
      float miter_length = max(half_thickness / max(dot(miter_direction, N1), 0.1), half_thickness);
      
      vec4 miter = miter_direction * miter_length;
      float total_width = length(miter) * 2;

      if(total_width > miter_limit) {
          vec4 new_miter_direction = (N1 * (1-uv.x)) + (N0 * uv.x);
          miter = new_miter_direction * half_thickness;
      }

      p = curr + (v * miter);
    } 

    p = screenToClip(p, screen_width, screen_heigth);
    return vec4(p.xy, NDC_curr.z / NDC_curr.w, 1.0);
}

#endif
