$input v_color
#include <bgfx_shader.sh>

void main() {
    gl_FragColor = v_color;

    // if(gl_PrimitiveID % 2 == 0) {
    //   gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    // } else {
    //   gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    // }
}
