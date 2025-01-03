#include "bgfx_compute.sh"

BUFFER_RO(segmentsBuffer,    float,  0);
BUFFER_WO(vertexBuffer,      float,  1);
BUFFER_WO(indexBuffer,       uint,   2);

#define p(pos)      vec3(segmentsBuffer[0 + ((pos) * 7)], segmentsBuffer[1 + ((pos) * 7)], segmentsBuffer[2 + ((pos) * 7)])
#define color(pos)  vec4(segmentsBuffer[3 + ((pos) * 7)], segmentsBuffer[4 + ((pos) * 7)], segmentsBuffer[5 + ((pos) * 7)], segmentsBuffer[6 + ((pos) * 7)])

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 48) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 12);

    vec3 p0 =    p((gl_WorkGroupID.x * 2));
    vec3 p1 =    p((gl_WorkGroupID.x * 2) + 1);
    vec4 color = color((gl_WorkGroupID.x * 2) + (1 - ((gl_LocalInvocationID.x + 1) % 2)));

    vertexBuffer[baseIndex]     = p0.x;
    vertexBuffer[baseIndex + 1] = p0.y;
    vertexBuffer[baseIndex + 2] = p0.z;

    vertexBuffer[baseIndex + 3] = p1.x;
    vertexBuffer[baseIndex + 4] = p1.y;
    vertexBuffer[baseIndex + 5] = p1.z;

    vertexBuffer[baseIndex + 6] = color.x;
    vertexBuffer[baseIndex + 7] = color.y;
    vertexBuffer[baseIndex + 8] = color.z;
    vertexBuffer[baseIndex + 9] = color.w;

    vertexBuffer[baseIndex + 10] = gl_LocalInvocationID.x;
    vertexBuffer[baseIndex + 11] = gl_LocalInvocationID.y;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[(6 * gl_WorkGroupID.x) + 0] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 1] = (gl_WorkGroupID.x * 4) + 3;
        indexBuffer[(6 * gl_WorkGroupID.x) + 2] = (gl_WorkGroupID.x * 4) + 1;

        indexBuffer[(6 * gl_WorkGroupID.x) + 3] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 4] = (gl_WorkGroupID.x * 4) + 2;
        indexBuffer[(6 * gl_WorkGroupID.x) + 5] = (gl_WorkGroupID.x * 4) + 3;
    }

}