#ifndef UTILS
#define UTILS

#include <bgfx_shader.sh>

vec4 screenToClip(vec4 coordinate, float screen_widht, float screen_heigth) {
    return vec4(
                  (2 * coordinate.x / screen_widht), 
                  (2 * coordinate.y / screen_heigth), 
                  coordinate.z, 
                  coordinate.w
                );
}

vec4 clipToScreen(vec4 coordinate, float screen_widht, float screen_heigth) {
    return vec4(
                  (coordinate.x * screen_widht) / 2, 
                  (coordinate.y * screen_heigth) / 2, 
                  coordinate.z, 
                  coordinate.w
                );
}

#endif