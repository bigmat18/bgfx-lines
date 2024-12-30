$input v_color, v_uv, v_length
#include <bgfx_shader.sh>

uniform vec4 u_data1;
uniform vec4 u_data2;

#define u_thickness           u_data1.w

#define u_leftCap             u_data2.x
#define u_rigthCap            u_data2.y

void main() {
	float d = -1;
    vec4 color = v_color;

	if(v_uv.x < 0) {
        if(u_leftCap == 2)
            d = length(vec2(v_uv.xy)) - (u_thickness/2);

	} else if(v_uv.x > v_length) {
        if(u_rigthCap == 2)
            d = length(vec2(v_uv.xy) - vec2(v_length, 0)) - (u_thickness/2);
	} 

	if(d > 0)
		discard;
	else
		gl_FragColor = color;

}