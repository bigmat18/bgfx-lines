#include "bgfx_compute.sh"

BUFFER_RO(segmentsBuffer,    float,  0);
IMAGE2D_WO(textureBuffer,    rgba32f, 1);

#define p0(pos)    vec3(segmentsBuffer[0 + (pos * 10)], segmentsBuffer[1 + (pos * 10)], segmentsBuffer[2 + (pos * 10)])
#define p1(pos)    vec3(segmentsBuffer[3 + (pos * 10)], segmentsBuffer[4 + (pos * 10)], segmentsBuffer[5 + (pos * 10)])
#define color(pos) vec4(segmentsBuffer[6 + (pos * 10)], segmentsBuffer[7 + (pos * 10)], segmentsBuffer[8 + (pos * 10)], segmentsBuffer[9 + (pos * 10)])

NUM_THREADS(1, 1, 1)
void main() {
    float baseIndex = gl_WorkGroupID.x * 3;

    vec3 p0 = p0(gl_WorkGroupID.x);
    vec3 p1 = p1(gl_WorkGroupID.x);
    vec4 color = color(gl_WorkGroupID.x);

    imageStore(textureBuffer, ivec2(baseIndex, 0),     vec4(p0.xyz, 0.0));
    imageStore(textureBuffer, ivec2(baseIndex + 1, 0), vec4(p1.xyz, 0.0));
    imageStore(textureBuffer, ivec2(baseIndex + 2, 0), color);
}