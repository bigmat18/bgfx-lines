$input v_uv, v_color, v_normal, v_thickness
#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_length;

#define u_linelength    u_length.x
#define u_antialias     u_data.z
#define u_thickness     u_data.w


void main() {
	float d = 0;
  float width = u_thickness / 2.0 - u_antialias;

  vec4 color = v_color;

/*
  if (v_normal.z < 0)
    color = 0.75*vec4(pow(abs(v_normal.z),0.5), 1.0, 1.0, 1.0);
*/

  if(v_uv.x < 0) {
    d = length(v_uv) - width;

  } else if(v_uv.x >= u_linelength) {

    d = length(v_uv - vec4(u_linelength, 0.0, 0.0, 0.0)) - width;
    
  } else {
    d = abs(v_uv.y) - width;
  }


	if(d < 0) {
  		gl_FragColor = color;
	} else {
      d /= u_antialias;
		  d = exp(-d * d); 
		  gl_FragColor = vec4(color.xyz, d);
	}
}
