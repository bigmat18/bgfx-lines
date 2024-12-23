#include "bgfx_compute.sh"

BUFFER_RO(segmentsBuffer,        float,  0);
IMAGE2D_WO(textureBuffer,        rgba32f,  1);

uniform vec4 u_IndirectData;
#define maxTextureSize u_IndirectData.x

#define p0(pos)    vec3(segmentsBuffer[0 + (pos * 10)], segmentsBuffer[1 + (pos * 10)], segmentsBuffer[2 + (pos * 10)])
#define p1(pos)    vec3(segmentsBuffer[3 + (pos * 10)], segmentsBuffer[4 + (pos * 10)], segmentsBuffer[5 + (pos * 10)])
#define color(pos) vec4(segmentsBuffer[6 + (pos * 10)], segmentsBuffer[7 + (pos * 10)], segmentsBuffer[8 + (pos * 10)], segmentsBuffer[9 + (pos * 10)])

NUM_THREADS(1, 1, 1)
void main() {
    vec3 p0 = p0(gl_WorkGroupID.x);
    vec3 p1 = p1(gl_WorkGroupID.x);
    vec4 color = color(gl_WorkGroupID.x);
    
    uint p0_Y = (gl_WorkGroupID.x * 3) / maxTextureSize;
    uint p0_X = (gl_WorkGroupID.x * 3) - (p0_Y * maxTextureSize);
    
    uint p1_Y = ((gl_WorkGroupID.x * 3) + 1) / maxTextureSize;
    uint p1_X = ((gl_WorkGroupID.x * 3) + 1) - (p1_Y * maxTextureSize);

    uint color_Y = ((gl_WorkGroupID.x * 3) + 2) / maxTextureSize;
    uint color_X = ((gl_WorkGroupID.x * 3) + 2) - (color_Y * maxTextureSize);

    imageStore(textureBuffer, ivec2(p0_X, p0_Y),       vec4(p0, 0.0));
    imageStore(textureBuffer, ivec2(p1_X, p1_Y),       vec4(p1, 0.0));
    imageStore(textureBuffer, ivec2(color_X, color_Y), color);
}