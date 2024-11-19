#include "bgfx_compute.sh"

BUFFER_RO(segmentsBuffer,    float,  0);
BUFFER_WO(vertexBuffer,      float,  1);
BUFFER_WO(indexBuffer,       uint,   2);

#define p0(pos)    vec3(segmentsBuffer[0 + (pos * 10)], segmentsBuffer[1 + (pos * 10)], segmentsBuffer[2 + (pos * 10)])
#define p1(pos)    vec3(segmentsBuffer[3 + (pos * 10)], segmentsBuffer[4 + (pos * 10)], segmentsBuffer[5 + (pos * 10)])
#define color(pos) vec4(segmentsBuffer[6 + (pos * 10)], segmentsBuffer[7 + (pos * 10)], segmentsBuffer[8 + (pos * 10)], segmentsBuffer[9 + (pos * 10)])

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 44) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 11);

    vec3 p0 =    p0(gl_WorkGroupID.x);
    vec3 p1 =    p1(gl_WorkGroupID.x);
    vec4 color = color(gl_WorkGroupID.x);

    float u = 2.0 * gl_LocalInvocationID.x - 1.0;
    float v = 2.0 * gl_LocalInvocationID.y - 1.0;

    vertexBuffer[baseIndex + (gl_LocalInvocationID.x * 3)]           = p0.x;
    vertexBuffer[baseIndex + (gl_LocalInvocationID.x * 3) + 1]       = p0.y;
    vertexBuffer[baseIndex + (gl_LocalInvocationID.x * 3) + 2]       = p0.z;

    vertexBuffer[baseIndex + ((1 - gl_LocalInvocationID.x) * 3)]     = p1.x;
    vertexBuffer[baseIndex + ((1 - gl_LocalInvocationID.x) * 3) + 1] = p1.y;
    vertexBuffer[baseIndex + ((1 - gl_LocalInvocationID.x) * 3) + 2] = p1.z;

    vertexBuffer[baseIndex + 6] = color.x;
    vertexBuffer[baseIndex + 7] = color.y;
    vertexBuffer[baseIndex + 8] = color.z;
    vertexBuffer[baseIndex + 9] = color.w;

    vertexBuffer[baseIndex + 10] = u * v;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[(6 * gl_WorkGroupID.x) + 0] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 1] = (gl_WorkGroupID.x * 4) + 1;
        indexBuffer[(6 * gl_WorkGroupID.x) + 2] = (gl_WorkGroupID.x * 4) + 2;

        indexBuffer[(6 * gl_WorkGroupID.x) + 3] = (gl_WorkGroupID.x * 4) + 1;
        indexBuffer[(6 * gl_WorkGroupID.x) + 4] = (gl_WorkGroupID.x * 4) + 3;
        indexBuffer[(6 * gl_WorkGroupID.x) + 5] = (gl_WorkGroupID.x * 4) + 2;

    }

}