$input a_position
$output v_color

#include <bgfx_shader.sh>

void main() {
    v_color = vec4(1.0, 0.0, 0.0, 1.0);
    gl_Position = vec4(a_position.xy, 0.0, 1.0);
}
