$input v_p, v_p0, v_p1, v_color
#include <bgfx_shader.sh>

uniform vec4 u_data;
#define u_thickness     u_data.w

void main() {
	float d = 0;

	if (v_p.x > length(v_p1 - v_p0)) {
		d = length(v_p - v_p1) - u_thickness/2.0;
	} else if( v_p.x < 0) {
		d = length(v_p - v_p0) - u_thickness/2.0;
	} else {
		d = abs(v_p.y) - u_thickness/2.0;
	}

	if(d < 0) {
		gl_FragColor = vec4(v_color.xyz, v_color.w);
	} else {
		d = exp(-d); 
		gl_FragColor = vec4(v_color.xyz, d * v_color.w);
	}
}