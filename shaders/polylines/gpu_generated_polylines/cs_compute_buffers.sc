#include <bgfx_compute.sh>

BUFFER_RO(pointsBuffer,              vec4,  0);
BUFFER_WO(vertexBuffer,              vec4,  1);
BUFFER_WO(segmentsIndexBuffer,       uint,   2);
BUFFER_WO(joinsIndexBuffer,          uint,   3);

uniform vec4 u_numWorksGroups;
#define numWorksGroups u_numWorksGroups.x

#define get_float_value(pos) pointsBuffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)        vec3(get_float_value(((pos) * 7) + 0), get_float_value(((pos) * 7) + 1), get_float_value(((pos) * 7) + 2))
#define color(pos)    get_float_value(((pos) * 7) + 3)
#define normal(pos)   vec3(get_float_value(((pos) * 7) + 4), get_float_value(((pos) * 7) + 5), get_float_value(((pos) * 7) + 6)) 

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 16) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 4);
    uint actualPoint = gl_WorkGroupID.x + gl_LocalInvocationID.x;
    uint numPoints = numWorksGroups;

    vec3 prev       = p(actualPoint - sign(actualPoint));
    vec3 curr       = p(actualPoint);
    vec3 next       = p(actualPoint + sign(numPoints - actualPoint));
    float color     = color(actualPoint);
    vec3 normal     = normal(actualPoint);

    vertexBuffer[baseIndex] = vec4(prev.x, prev.y, prev.z, curr.x);
    vertexBuffer[baseIndex + 1] = vec4(curr.y, curr.z, next.x, next.y);
    vertexBuffer[baseIndex + 2] = vec4(next.z, uintBitsToFloat(bitfieldReverse(floatBitsToUint(color))), normal.x, normal.y);
    vertexBuffer[baseIndex + 3] = vec4(normal.z, gl_LocalInvocationID.x, gl_LocalInvocationID.y, 0);

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