#include "bgfx_compute.sh"
#include "../../utils.sh"

BUFFER_RO(pointsBuffer,   float,    0);
IMAGE2D_WO(textureBuffer, rgba32f,  1);
BUFFER_RW(indirectBuffer, uvec4,    2);

uniform vec4 u_IndirectData;

#define maxTextureSize          u_IndirectData.x
#define instancingNum           u_IndirectData.y

#define p(pos)        vec3(pointsBuffer[((pos) * 7) + 0], pointsBuffer[((pos) * 7) + 1], pointsBuffer[((pos) * 7) + 2])
#define color(pos)    pointsBuffer[((pos) * 7) + 3]
#define normal(pos)   vec3(pointsBuffer[((pos) * 7) + 4], pointsBuffer[((pos) * 7) + 5], pointsBuffer[((pos) * 7) + 6])   

NUM_THREADS(1, 1, 1)
void main() {
    vec3 p0          = p((gl_WorkGroupID.x * 2));
    vec3 p1          = p((gl_WorkGroupID.x * 2) + 1);
    float color0     = color((gl_WorkGroupID.x * 2));
    float color1     = color((gl_WorkGroupID.x * 2) + 1);
    vec3 normal0     = normal((gl_WorkGroupID.x * 2));
    vec3 normal1     = normal((gl_WorkGroupID.x * 2) + 1);

    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) , maxTextureSize),    vec4(p0.xyz, color0));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 1, maxTextureSize), vec4(p1.xyz, color1));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 2, maxTextureSize), vec4(normal0.xyz, 0));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 3, maxTextureSize), vec4(normal1.xyz, 0));
    
    if(gl_WorkGroupID.x == 0)
	    drawIndexedIndirect(indirectBuffer, 0, 6, floor(instancingNum), 0, 0, 0);
} 