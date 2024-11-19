$input v_color
#include <bgfx_shader.sh>
SAMPLER2D(u_Texture,  0);

void main()
{
	gl_FragColor = v_color;
}