$input a_position
$output v_color

#include <bgfx_compute.sh>
#include "../../lines.sh"

BUFFER_RO(segmentsBuffer, float, 1);

#define p0(pos)    vec4(segmentsBuffer[0 + ((pos) * 10)], segmentsBuffer[1 + ((pos) * 10)], segmentsBuffer[2 + ((pos) * 10)], 0.0)
#define p1(pos)    vec4(segmentsBuffer[3 + ((pos) * 10)], segmentsBuffer[4 + ((pos) * 10)], segmentsBuffer[5 + ((pos) * 10)], 0.0)
#define color(pos) vec4(segmentsBuffer[6 + ((pos) * 10)], segmentsBuffer[7 + ((pos) * 10)], segmentsBuffer[8 + ((pos) * 10)], segmentsBuffer[9 + ((pos) * 10)])

uniform vec4 u_data1;
uniform vec4 u_data2;

#define uv                    a_position
#define u_screenWidth         u_data1.x
#define u_screenHeigth        u_data1.y
#define u_thickness           u_data1.z

void main() {
    vec4 p0 =    p0(gl_InstanceID);
    vec4 p1 =    p1(gl_InstanceID);
    vec4 color = color(gl_InstanceID);

    v_color = color;
    gl_Position = calculateLines(p0, p1, uv, u_thickness, u_screenWidth, u_screenHeigth);
}
