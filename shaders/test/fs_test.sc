$input v_color
#include <bgfx_shader.sh>

void main() {
    if(gl_PrimitiveID % 2 == 0) {
        gl_FragColor = vec4(0, 0, 1, 1);
    } else {
        gl_FragColor = v_color;
    }
}