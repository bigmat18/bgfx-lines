$input v_color0, v_p
#include <bgfx_shader.sh>

uniform vec4 u_antialias;
uniform vec4 u_thickness;
uniform vec4 u_resolution;
uniform vec4 u_alpha;

uniform vec4 u_p0;
uniform vec4 u_p1;

void main() {
	float d = 0;
	float b = 0;
	if( v_p.x < 0) {
		d = length(v_p - u_p0) - u_thickness.x/2.0;
		b = 1;
	} else if (v_p.x > length(u_p1 - u_p0)) {
		d = length(v_p - u_p1) - u_thickness.x/2.0;
		b = 1;
	} else {
		d = abs(v_p.y) - u_thickness.x/2.0;
	}

	if(d < 0) {
		gl_FragColor = vec4(b, 1.0, 0.0, u_alpha);
	} else {
		d = exp(-d);
		gl_FragColor = vec4(b, 1.0, 0.0, d * u_alpha);
	}
}