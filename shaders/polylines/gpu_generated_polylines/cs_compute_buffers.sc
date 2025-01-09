#include <bgfx_compute.sh>

BUFFER_RO(pointsBuffer,              float,  0);
BUFFER_WO(vertexBuffer,              float,  1);
BUFFER_WO(segmentsIndexBuffer,       uint,   2);
BUFFER_WO(joinsIndexBuffer,          uint,   3);

uniform vec4 u_numWorksGroups;
#define numWorksGroups u_numWorksGroups.x

#define p(pos)        vec3(pointsBuffer[((pos) * 7) + 0], pointsBuffer[((pos) * 7) + 1], pointsBuffer[((pos) * 7) + 2])
#define color(pos)    pointsBuffer[((pos) * 7) + 3]
#define normal(pos)   vec3(pointsBuffer[((pos) * 7) + 4], pointsBuffer[((pos) * 7) + 5], pointsBuffer[((pos) * 7) + 6]) 

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 60) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 15);
    uint actualPoint = gl_WorkGroupID.x + gl_LocalInvocationID.x;
    uint numPoints = numWorksGroups;

    vec3 prev       = p(actualPoint - sign(actualPoint));
    vec3 curr       = p(actualPoint);
    vec3 next       = p(actualPoint + sign(numPoints - actualPoint));
    float color     = color(actualPoint);
    vec3 normal     = normal(actualPoint);

    vertexBuffer[baseIndex + 0] = prev.x;
    vertexBuffer[baseIndex + 1] = prev.y;
    vertexBuffer[baseIndex + 2] = prev.z;

    vertexBuffer[baseIndex + 3] = curr.x;
    vertexBuffer[baseIndex + 4] = curr.y;
    vertexBuffer[baseIndex + 5] = curr.z;

    vertexBuffer[baseIndex + 6] = next.x;
    vertexBuffer[baseIndex + 7] = next.y;
    vertexBuffer[baseIndex + 8] = next.z;

    vertexBuffer[baseIndex + 9]  = uintBitsToFloat(bitfieldReverse(floatBitsToUint(color)));

    vertexBuffer[baseIndex + 10] = normal.x;
    vertexBuffer[baseIndex + 11] = normal.y;
    vertexBuffer[baseIndex + 12] = normal.z;

    vertexBuffer[baseIndex + 13] = gl_LocalInvocationID.x;
    vertexBuffer[baseIndex + 14] = gl_LocalInvocationID.y;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6)]     = (gl_WorkGroupID.x * 4);
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 1] = (gl_WorkGroupID.x * 4) + 3;
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 2] = (gl_WorkGroupID.x * 4) + 1;

        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 3] = (gl_WorkGroupID.x * 4);
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 4] = (gl_WorkGroupID.x * 4) + 2;
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 5] = (gl_WorkGroupID.x * 4) + 3;

        if(gl_WorkGroupID.x != numWorksGroups - 1) {
            joinsIndexBuffer[(gl_WorkGroupID.x * 6)]      = (gl_WorkGroupID.x * 4) + 3;
            joinsIndexBuffer[(gl_WorkGroupID.x * 6) + 1]  = (gl_WorkGroupID.x * 4) + 4;
            joinsIndexBuffer[(gl_WorkGroupID.x * 6) + 2]  = (gl_WorkGroupID.x * 4) + 5;

            joinsIndexBuffer[(gl_WorkGroupID.x * 6) + 3]  = (gl_WorkGroupID.x * 4) + 4;
            joinsIndexBuffer[(gl_WorkGroupID.x * 6) + 4]  = (gl_WorkGroupID.x * 4) + 2;
            joinsIndexBuffer[(gl_WorkGroupID.x * 6) + 5]  = (gl_WorkGroupID.x * 4) + 5;
        }
    }

}