$input a_position
$output v_color

#include <bgfx_shader.sh>

uniform vec4 u_color;

void main() {
    v_color = u_color;
    gl_Position = mul(u_modelViewProj, vec4(a_position.xyz, 1.0));
}