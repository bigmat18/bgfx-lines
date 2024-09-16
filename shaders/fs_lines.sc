$input v_uv, v_color
#include <bgfx_shader.sh>

uniform vec4 u_prev;
uniform vec4 u_curr;
uniform vec4 u_next;
uniform vec4 u_data;
uniform vec4 u_color;
uniform vec4 u_length;

#define lenght          u_length.x
#define u_width         u_data.x
#define u_heigth        u_data.y
#define u_antialias     u_data.z
#define u_thickness     u_data.w


void main() {
	float d = 0;
    float width = u_thickness / 2.0 - u_antialias;

    if(v_uv.x < 0) {

    } else if (v_uv.x >= length)

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
