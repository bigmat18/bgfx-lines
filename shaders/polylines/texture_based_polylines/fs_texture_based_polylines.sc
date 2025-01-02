$input v_color, v_uv, v_length
#include <bgfx_shader.sh>

uniform vec4 u_data1;
uniform vec4 u_data2;

#define u_thickness           u_data1.w

#define u_leftCap             u_data2.x
#define u_rigthCap            u_data2.y
#define u_join           	  u_data2.z

void main() {
	float d = -1;
    float color = 0;

	if(v_uv.x < 0) {
        if(u_leftCap == 2 || u_join == 0)
            d = length(vec2(v_uv.xy)) - (u_thickness/2);
		color = 1;

	} else if(v_uv.x > v_length) {
        if(u_rigthCap == 2 || u_join == 0)
            d = length(vec2(v_uv.xy) - vec2(v_length, 0)) - (u_thickness/2);
		color = 1;
	} 

	if(d > 0)
		discard;
	else {
		// if(color != 1) {
		// 	if(gl_PrimitiveID % 2 == 0) {
		// 		gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);;
		// 	} else {
		// 		gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);;
		// 	}
		// } else {
		// 	gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
		// }

		gl_FragColor = v_color;
	}

}