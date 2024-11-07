#include "bgfx_compute.sh"

BUFFER_RO(pointsBuffer,    vec3, 0);
BUFFER_WO(vertexBuffer,    float, 1);
BUFFER_WO(indexBuffer,     float, 2);

NUM_THREADS(2, 2, 1)
void main() {
    vertexBuffer[gl_GlobalInvocationID.x] =     pointsBuffer[gl_GlobalInvocationID.x].x;
    vertexBuffer[gl_GlobalInvocationID.x + 1] = pointsBuffer[gl_GlobalInvocationID.x].y;
    vertexBuffer[gl_GlobalInvocationID.x + 2] = pointsBuffer[gl_GlobalInvocationID.x].z;

    vertexBuffer[gl_GlobalInvocationID.x + 3] = pointsBuffer[gl_GlobalInvocationID.x + 1].x;
    vertexBuffer[gl_GlobalInvocationID.x + 4] = pointsBuffer[gl_GlobalInvocationID.x + 1].y;
    vertexBuffer[gl_GlobalInvocationID.x + 5] = pointsBuffer[gl_GlobalInvocationID.x + 1].z;

    vertexBuffer[gl_GlobalInvocationID.x + 6] = gl_LocalInvocationID.x;
    vertexBuffer[gl_GlobalInvocationID.x + 7] = gl_LocalInvocationID.y;
}