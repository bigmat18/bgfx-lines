$input v_color, v_uv, v_prev, v_curr, v_next
#include <bgfx_shader.sh>

uniform vec4 u_data;

#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_linelength      u_data.z
#define u_thickness       u_data.w

void main() {
    gl_FragColor = v_color;

    // if(gl_PrimitiveID % 2 == 0) {
    //   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    // } else {
    //   gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    // }
}
