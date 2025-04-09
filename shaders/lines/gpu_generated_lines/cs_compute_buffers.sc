#include "bgfx_compute.sh"
#include "../../utils.sh"

BUFFER_RO(pointsBuffer,      vec4,  0);
BUFFER_WO(vertexBuffer,      vec4,  1);
BUFFER_WO(indexBuffer,       uint,   2);

#define p(pos)        vec3(pointsBuffer[((pos) * 7) + 0], pointsBuffer[((pos) * 7) + 1], pointsBuffer[((pos) * 7) + 2])
#define color(pos)    pointsBuffer[((pos) * 7) + 3]
#define normal(pos)   vec3(pointsBuffer[((pos) * 7) + 4], pointsBuffer[((pos) * 7) + 5], pointsBuffer[((pos) * 7) + 6])     

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 48) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 12);

    vec3 p0        = p((gl_WorkGroupID.x * 2));
    vec3 p1        = p((gl_WorkGroupID.x * 2) + 1);
    float color    = color((gl_WorkGroupID.x * 2) + (1 - (uint(gl_LocalInvocationID.x + 1) % 2)));
    vec3 normal    = normal((gl_WorkGroupID.x * 2) + (1 - (uint(gl_LocalInvocationID.x + 1) % 2)));

    vertexBuffer[baseIndex]     = vec4(p0.x, p0.y, p0.z, p1.x);
    vertexBuffer[baseIndex + 1] = vec4(p1.y, p1.z, uintBitsToFloat(bitfieldReverse(floatBitsToUint(color))), normal.x);
    vertexBuffer[baseIndex + 2] = vec4(normal.y, normal.z, gl_LocalInvocationID.x, gl_LocalInvocationID.y);

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[(6 * gl_WorkGroupID.x) + 0] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 1] = (gl_WorkGroupID.x * 4) + 3;
        indexBuffer[(6 * gl_WorkGroupID.x) + 2] = (gl_WorkGroupID.x * 4) + 1;

        indexBuffer[(6 * gl_WorkGroupID.x) + 3] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 4] = (gl_WorkGroupID.x * 4) + 2;
        indexBuffer[(6 * gl_WorkGroupID.x) + 5] = (gl_WorkGroupID.x * 4) + 3;
    }

}