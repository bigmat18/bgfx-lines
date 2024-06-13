$input v_color0, v_p
#include <bgfx_shader.sh>

uniform vec4 u_antialias;
uniform vec4 u_thickness;

uniform vec4 u_p0;
uniform vec4 u_p1;

void main() {
	float d = 0;
	if( v_p.x < 0) {
		d = length(v_p - u_p0) - u_thickness.x/2.0 + u_antialias.x/2.0;
	} else if (v_p.x > length(u_p1 - u_p0)) {
		d = length(v_p - u_p1) - u_thickness.x/2.0 + u_antialias.x/2.0;
	} else {
		d = abs(v_p.y) - u_thickness.x/2.0 + u_antialias.x/2.0;
	}

	if(d < 0) {
		gl_FragColor = vec4(v_color0.xyz, 1.0);
	} else if (d < u_antialias.x) {
		d = exp(-d * d);
		gl_FragColor = vec4(v_color0.xyz, d);
	}

}