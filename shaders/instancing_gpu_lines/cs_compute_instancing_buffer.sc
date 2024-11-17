#include "bgfx_compute.sh"

BUFFER_RO(segmentsBuffer,    float,  0);
SAMPLER2D(s_Texture,  0);

#define p0(pos)    vec3(segmentsBuffer[0 + (pos * 10)], segmentsBuffer[1 + (pos * 10)], segmentsBuffer[2 + (pos * 10)])
#define p1(pos)    vec3(segmentsBuffer[3 + (pos * 10)], segmentsBuffer[4 + (pos * 10)], segmentsBuffer[5 + (pos * 10)])
#define color(pos) vec4(segmentsBuffer[6 + (pos * 10)], segmentsBuffer[7 + (pos * 10)], segmentsBuffer[8 + (pos * 10)], segmentsBuffer[9 + (pos * 10)])

NUM_THREADS(1, 1, 1)
void main() {
    uint = gl_WorkGroupID.x * 12;

    vec3 p0 = p0(gl_WorkGroupID.x);
    vec3 p1 = p1(gl_WorkGroupID.x);
    vec3 color = color(gl_WorkGroupID.x);

    s_Texture[gl_WorkGroupID.x] = p0.x;
    s_Texture[gl_WorkGroupID.x + 1] = p0.y;
    s_Texture[gl_WorkGroupID.x + 2] = p0.z;
    s_Texture[gl_WorkGroupID.x + 3] = 0.0;

    s_Texture[gl_WorkGroupID.x + 4] = p1.x;
    s_Texture[gl_WorkGroupID.x + 5] = p1.y;
    s_Texture[gl_WorkGroupID.x + 6] = p1.z;
    s_Texture[gl_WorkGroupID.x + 7] = 0.0;

    s_Texture[gl_WorkGroupID.x + 8] = color.x;
    s_Texture[gl_WorkGroupID.x + 9] = color.y;
    s_Texture[gl_WorkGroupID.x + 10] = color.z;
    s_Texture[gl_WorkGroupID.x + 11] = color.w;
}