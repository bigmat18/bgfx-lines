$input a_position, a_texcoord0, a_color0, a_texcoord1
$output v_color

#define p0                    a_position
#define p1                    a_texcoord0
#define uv                    a_texcoord1

#include <bgfx_shader.sh>

void main() {
    v_color = a_color0;
    gl_Position = vec4(uv.x, uv.y, 0.0, 1.0);
}
