#ifndef POLYLINES
#define POLYLINES

#include <bgfx_shader.sh>
#include "utils.sh"

vec4 calculatePolylines(vec4 prev, vec4 curr, vec4 next, vec2 uv, float thickness, float miter_limit, 
                        float screen_width, float screen_heigth, float leftCap, float rightCap, float join,
                        bool is_start, bool is_end) {
    float half_thickness = thickness / 2.0;

    vec4 T0 = vec4(normalize(curr.xy - prev.xy).xy, 0.0, 0.0);
    vec4 N0 = vec4(-T0.y , T0.x, 0.0, 0.0);

    vec4 T1 = vec4(normalize(next.xy - curr.xy).xy, 0.0, 0.0);
    vec4 N1 = vec4(-T1.y, T1.x, 0.0, 0.0);

    vec4 p;

    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    if(is_start) {
    
      p = curr + (v * half_thickness * N1) + (u * T1 * half_thickness * sign(leftCap));

    } else if (is_end) {

      p = curr + (v * half_thickness * N0) + (u * T0 * half_thickness * sign(rightCap));

    } else {

        vec4 miter_direction = normalize(N0 + N1);
        float cos_theta = dot(miter_direction, N1);
        float miter_length = max(half_thickness / max(cos_theta, 0.1), half_thickness);
      
        vec4 miter = miter_direction * miter_length;
        float total_width = length(miter) * 2;

        vec4 new_miter_direction = (N1 * (1-uv.x)) + (N0 * uv.x);
        vec4 miter_plane = new_miter_direction * half_thickness;

        float min_threshold = miter_limit;
        float max_threshold = miter_limit * 1.5;
        float t = smoothstep(min_threshold, max_threshold, total_width);

        vec4 final_miter;
        if (join == 2) {
          final_miter = mix(miter, miter_plane, t);
        } else {
          final_miter =  miter_plane;
        }

        #if 0
          if(total_width > miter_limit) {
              vec4 new_miter_direction = (N1 * (1-uv.x)) + (N0 * uv.x);
              miter = new_miter_direction * half_thickness;
          }
        #endif

        p = curr + (v * final_miter) + (u * half_thickness * ((T1 * (1-uv.x)) + (T0 * uv.x)) * (1 - sign(join)));
    } 

    p = screenToClip(p, screen_width, screen_heigth);
    return vec4(p.xy, curr.z / curr.w, 1.0);
}

vec4 calculatePolylinesUV(vec4 prev, vec4 curr, vec4 next, vec2 uv, float thickness, float length_px, float leftCap, float rigthCap, float join) {
    vec4 T = vec4(1.0, 0.0, 0.0, 0.0);
    vec4 N = vec4(0.0, 1.0, 0.0, 0.0);

    float width_px = thickness / 2.0;

    float u = 2.0 * uv.x - 1.0;
    float v = 2.0 * uv.y - 1.0;

    vec4 final_uv = (uv.x * length_px * T) + (v * N * width_px);
    float activeCaps = sign(leftCap) * (1 - sign(length(curr - prev))) + sign(rigthCap) * (1 - sign(length(next - curr)));
    float activeJoin = (1 - sign(join)) * sign(length(curr - prev)) * sign(length(next - curr));
    return final_uv + (u * T * width_px * (activeCaps)) + (u * T * width_px * (activeJoin));
}

vec4 calculatePolylinesColor(vec2 uv, float thickness, float totalLength, float leftCap, float rigthCap, float join, vec4 finalColor, float is_start_end) {
  	float d = -1;
    float color = 0;
    float width_px = thickness / 2;

    if(uv.x < 0) {
        float square_cap    = (width_px - max(abs(uv.x), abs(uv.y))) * (1 - sign(abs(leftCap - 1)));
        float round_cap     = (width_px - length(uv))  			         * (1 - sign(abs(leftCap - 2)));
        float triangle_cap  = (width_px - (abs(uv.x) + abs(uv.y)))   * (1 - sign(abs(leftCap - 3)));

        d = (square_cap + round_cap + triangle_cap) * sign(sign(is_start_end) + sign(join)) + (width_px - length(uv)) * (1 - sign(sign(is_start_end) + sign(join)));
    } else if(uv.x > totalLength) {
        float square_cap    = (width_px - max(abs(uv.x - totalLength), abs(uv.y))) * (1 - sign(abs(rigthCap - 1)));
        float round_cap     = (width_px - length(uv - vec2(totalLength, 0)))       * (1 - sign(abs(rigthCap - 2)));
        float triangle_cap  = (width_px - (abs(uv.x - totalLength) + abs(uv.y)))   * (1 - sign(abs(rigthCap - 3)));

        d = (square_cap + round_cap + triangle_cap) * sign(sign(is_start_end) + sign(join)) + (width_px - length(uv - vec2(totalLength, 0))) * (1 - sign(sign(is_start_end) + sign(join)));
    } else
        d = width_px - abs(uv.y);

	  if(d < 0)
		    return vec4(0);
	  else
		    return finalColor;
}

#endif
