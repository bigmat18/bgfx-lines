$input v_p, v_p0, v_p1
#include <bgfx_shader.sh>

uniform vec4 u_antialias;
uniform vec4 u_thickness;
uniform vec4 u_resolution;
uniform vec4 u_color;

void main() {
	float d = 0;

	if (v_p.x > length(v_p1 - v_p0)) {
		d = length(v_p - v_p1) - u_thickness.x/2.0;
	} else if( v_p.x < 0) {
		d = length(v_p - v_p0) - u_thickness.x/2.0;
	} else {
		d = abs(v_p.y) - u_thickness.x/2.0;
	}

	if(d < 0) {
		gl_FragColor = vec4(u_color.xyz, u_color.w);
	} else {
		d = exp(-d); 
		gl_FragColor = vec4(u_color.xyz, d * u_color.w);
	}
}