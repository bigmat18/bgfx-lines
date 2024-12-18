$input a_position
$output v_color

#include <bgfx_compute.sh>
#include "../../lines.sh"

uniform vec4 u_data;

#define uv                    a_position
#define u_screenWidth         u_data.x
#define u_screenHeigth        u_data.y
#define u_antialias           u_data.z
#define u_thickness           u_data.w

IMAGE2D_RO(textureBuffer, rgba32f, 0);


void main() {
    vec4 p0    = imageLoad(textureBuffer, vec2((gl_InstanceID * 3), 0));
    vec4 p1    = imageLoad(textureBuffer, vec2((gl_InstanceID * 3) + 1, 0));
    vec4 color = imageLoad(textureBuffer, vec2((gl_InstanceID * 3) + 2, 0));

    gl_Position = calculateLines(p0, p1, uv, u_thickness, u_screenWidth, u_screenHeigth);
    v_color = color;
}
