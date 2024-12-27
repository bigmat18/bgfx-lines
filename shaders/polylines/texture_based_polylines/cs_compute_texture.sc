#include "bgfx_compute.sh"

BUFFER_RO(pointsBuffer,          float,  0);
IMAGE2D_WO(textureBuffer,        rgba32f,  1);

uniform vec4 u_IndirectData;
#define maxTextureSize u_IndirectData.x

#define p(pos)    vec3(pointsBuffer[((pos) * 3)], pointsBuffer[((pos) * 3) + 1], pointsBuffer[((pos) * 3) + 2])

NUM_THREADS(1, 1, 1)
void main() {
    uint p0_Y = gl_WorkGroupID.x / maxTextureSize;
    uint p0_X = gl_WorkGroupID.x - (p0_Y * maxTextureSize);

    imageStore(textureBuffer, ivec2(p0_X, p0_Y), vec4(p(gl_WorkGroupID.x).xyz, 0));
}