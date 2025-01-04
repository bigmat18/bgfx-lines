$input v_color, v_uv, v_length
#include <bgfx_shader.sh>

uniform vec4 u_data1;
uniform vec4 u_data2;
uniform vec4 u_borderColor;

#define u_thickness           u_data1.z
#define u_leftCap             u_data1.w
#define u_rigthCap            u_data2.x
#define u_screenWidth         u_data1.x

#define u_antialias           u_data2.y
#define u_border              u_data2.z

void main() {
	float d = -1;
	float width_px = (u_thickness / 2) + u_antialias + u_border;

	if(v_uv.x < 0) {
		if(u_leftCap == 2)
			d = width_px - length(v_uv);
	} else if(v_uv.x > v_length) {
		if(u_rigthCap == 2)
			d = width_px - length(v_uv - vec4(v_length, 0, 0, 0));
	} else {
		d = width_px - abs(v_uv.y);
	}

	if(d > u_antialias + u_border) {
		gl_FragColor = v_color;
	} else if (d > u_border) {
		gl_FragColor = u_borderColor;
	} else if (d > 0) {
		gl_FragColor = u_borderColor;
	} else {
		if(v_uv.x < 0 && u_leftCap != 2) {
			if(abs(v_uv.x) < (u_thickness / 2) && abs(v_uv.y) < (u_thickness / 2)) {
				gl_FragColor = v_color;
			} else if(abs(v_uv.x) < (u_thickness / 2) + u_border && abs(v_uv.y) < (u_thickness / 2) + u_border) {
				gl_FragColor = u_borderColor;
			} else {
				gl_FragColor = u_borderColor;
			}
		} else if(v_uv.x > v_length && u_rigthCap != 2) {
			if(abs(v_uv.x - v_length) < (u_thickness / 2) && abs(v_uv.y) < (u_thickness / 2)) {
				gl_FragColor = v_color;
			} else if(abs(v_uv.x - v_length) < (u_thickness / 2) + u_border && abs(v_uv.y) < (u_thickness / 2) + u_border) {
				gl_FragColor = u_borderColor;
			} else {
				gl_FragColor = u_borderColor;
			}
		} else {
			discard;
		}
	}

}
