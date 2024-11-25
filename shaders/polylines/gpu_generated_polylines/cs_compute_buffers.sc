#include <bgfx_compute.sh>

BUFFER_RO(pointsBuffer,      float,  0);
BUFFER_WO(vertexBuffer,      float,  1);
BUFFER_WO(indexBuffer,       uint,   2);

uniform vec4 u_numWorksGroups;
#define numWorksGroups u_numWorksGroups.x

#define p(pos)    vec3(pointsBuffer[((pos) * 3)], pointsBuffer[((pos) * 3) + 1], pointsBuffer[((pos) * 3) + 2])

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 44) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 11);
    int actualPoint = gl_WorkGroupID.x + gl_LocalInvocationID.x;
    int numPoints = numWorksGroups;

    vec3 prev = p(actualPoint - sign(actualPoint));
    vec3 curr = p(actualPoint);
    vec3 next = p(actualPoint + sign(numPoints - actualPoint));

    vertexBuffer[baseIndex + 0] = prev.x;
    vertexBuffer[baseIndex + 1] = prev.y;
    vertexBuffer[baseIndex + 2] = prev.z;

    vertexBuffer[baseIndex + 3] = curr.x;
    vertexBuffer[baseIndex + 4] = curr.y;
    vertexBuffer[baseIndex + 5] = curr.z;

    vertexBuffer[baseIndex + 6] = next.x;
    vertexBuffer[baseIndex + 7] = next.y;
    vertexBuffer[baseIndex + 8] = next.z;

    vertexBuffer[baseIndex + 9] = gl_LocalInvocationID.x;
    vertexBuffer[baseIndex + 10] = gl_LocalInvocationID.y;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[(gl_WorkGroupID.x * 12)]     = (gl_WorkGroupID.x * 4);
        indexBuffer[(gl_WorkGroupID.x * 12) + 1] = (gl_WorkGroupID.x * 4) + 3;
        indexBuffer[(gl_WorkGroupID.x * 12) + 2] = (gl_WorkGroupID.x * 4) + 1;

        indexBuffer[(gl_WorkGroupID.x * 12) + 3] = (gl_WorkGroupID.x * 4);
        indexBuffer[(gl_WorkGroupID.x * 12) + 4] = (gl_WorkGroupID.x * 4) + 2;
        indexBuffer[(gl_WorkGroupID.x * 12) + 5] = (gl_WorkGroupID.x * 4) + 3;

        if(gl_WorkGroupID.x != numWorksGroups - 1) {
            indexBuffer[(gl_WorkGroupID.x * 12) + 6]  = (gl_WorkGroupID.x * 4) + 3;
            indexBuffer[(gl_WorkGroupID.x * 12) + 7]  = (gl_WorkGroupID.x * 4) + 4;
            indexBuffer[(gl_WorkGroupID.x * 12) + 8]  = (gl_WorkGroupID.x * 4) + 5;

            indexBuffer[(gl_WorkGroupID.x * 12) + 9]  = (gl_WorkGroupID.x * 4) + 4;
            indexBuffer[(gl_WorkGroupID.x * 12) + 10] = (gl_WorkGroupID.x * 4) + 2;
            indexBuffer[(gl_WorkGroupID.x * 12) + 11] = (gl_WorkGroupID.x * 4) + 5;
        }
    }

}