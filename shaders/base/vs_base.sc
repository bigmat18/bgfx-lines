$input a_position
$output v_color

#include <bgfx_shader.sh>

void main() {
    v_color = vec4(1,0,1,1);
    vec4 pos = mul(u_modelViewProj, vec4(a_position.xyz, 1.0));

    gl_Position = pos;
}