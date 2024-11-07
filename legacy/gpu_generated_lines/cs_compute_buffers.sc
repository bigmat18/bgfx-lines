#include "bgfx_compute.sh"

BUFFER_RO(pointsBuffer,    vec3, 0);
BUFFER_WO(vertexBuffer,    float, 1);
BUFFER_WO(indexBuffer,     uint, 2);

NUM_THREADS(2, 2, 1)
void main() {
    int baseIndex = (gl_GlobalInvocationID.x * 8) + (gl_LocalInvocationID.x + gl_LocalInvocationID.y * 2);

    vertexBuffer[baseIndex] =     pointsBuffer[gl_GlobalInvocationID.x].x;
    vertexBuffer[baseIndex + 1] = pointsBuffer[gl_GlobalInvocationID.x].y;
    vertexBuffer[baseIndex + 2] = pointsBuffer[gl_GlobalInvocationID.x].z;

    vertexBuffer[baseIndex + 3] = pointsBuffer[gl_GlobalInvocationID.x + 1].x;
    vertexBuffer[baseIndex + 4] = pointsBuffer[gl_GlobalInvocationID.x + 1].y;
    vertexBuffer[baseIndex + 5] = pointsBuffer[gl_GlobalInvocationID.x + 1].z;

    vertexBuffer[baseIndex + 6] = gl_LocalInvocationID.x;
    vertexBuffer[baseIndex + 7] = gl_LocalInvocationID.y;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[gl_GlobalInvocationID] = gl_GlobalInvocationID;
        indexBuffer[gl_GlobalInvocationID] = gl_GlobalInvocationID + 2;
        indexBuffer[gl_GlobalInvocationID] = gl_GlobalInvocationID + 1;

        indexBuffer[gl_GlobalInvocationID] = gl_GlobalInvocationID;
        indexBuffer[gl_GlobalInvocationID] = gl_GlobalInvocationID + 2;
        indexBuffer[gl_GlobalInvocationID] = gl_GlobalInvocationID + 3;
    }

}