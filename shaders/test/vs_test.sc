$input a_position
$output v_color

#include <bgfx_shader.sh>

void main() {
    v_color = vec4(1,0,0,1);
    gl_Position = a_position;
}