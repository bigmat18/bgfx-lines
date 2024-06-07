$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>

void main()
{
	float y = sqrt(pow(a_position.z, 2) - pow(a_position.x, 2) - 1);
	gl_Position = mul(u_modelViewProj, vec4(a_position.x, a_position.y, a_position.z, 1.0) );
	v_color0 = a_color0;
}
