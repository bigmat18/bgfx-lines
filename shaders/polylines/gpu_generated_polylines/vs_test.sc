$input a_position, a_texcoord0, a_texcoord1, a_texcoord2
$output v_color

uniform vec4 u_data;
uniform vec4 u_color;

#define a_prev            a_position
#define a_curr            a_texcoord0
#define a_next            a_texcoord1
#define a_uv              a_texcoord2 

#include <bgfx_shader.sh>

void main() {
    v_color = vec4(a_next.xyz, 1.0);
    gl_Position = vec4(a_uv.x, a_uv.y, 0.0, 1.0);
}
