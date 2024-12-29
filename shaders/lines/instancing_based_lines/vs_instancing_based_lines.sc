$input a_position, i_data0, i_data1, i_data2
$output v_color

#include <bgfx_shader.sh>
#include "../../lines.sh"

uniform vec4 u_data1;
uniform vec4 u_data2;

#define p0                    i_data0
#define p1                    i_data1
#define color                 i_data2
#define uv                    a_position

#define u_screenWidth         u_data1.x
#define u_screenHeigth        u_data1.y
#define u_thickness           u_data1.z

void main() {
    v_color = color;
    gl_Position = calculateLines(p0, p1, uv, u_thickness, u_screenWidth, u_screenHeigth);
}
