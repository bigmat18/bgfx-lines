#include "bgfx_compute.sh"
#include "../../utils.sh"

BUFFER_RO(segmentsBuffer, float,    0);
IMAGE2D_WO(textureBuffer, rgba32f,  1);
BUFFER_RW(indirectBuffer, uvec4,    2);

uniform vec4 u_IndirectData;

#define maxTextureSize          u_IndirectData.x
#define instancingNum           u_IndirectData.y

#define p(pos)      vec3(segmentsBuffer[0 + ((pos) * 7)], segmentsBuffer[1 + ((pos) * 7)], segmentsBuffer[2 + ((pos) * 7)])
#define color(pos)  vec4(segmentsBuffer[3 + ((pos) * 7)], segmentsBuffer[4 + ((pos) * 7)], segmentsBuffer[5 + ((pos) * 7)], segmentsBuffer[6 + ((pos) * 7)])

NUM_THREADS(1, 1, 1)
void main() {
    vec3 p0     = p((gl_WorkGroupID.x * 2));
    vec3 p1     = p((gl_WorkGroupID.x * 2) + 1);
    vec4 color0 = color((gl_WorkGroupID.x * 2));
    vec4 color1 = color((gl_WorkGroupID.x * 2) + 1);

    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) , maxTextureSize),    vec4(p0, 0.0));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 1, maxTextureSize), vec4(p1, 0.0));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 2, maxTextureSize), color0);
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 3, maxTextureSize), color1);
    
    if(gl_WorkGroupID.x == 0)
	    drawIndexedIndirect(indirectBuffer, 0, 6, floor(instancingNum), 0, 0, 0);
} 