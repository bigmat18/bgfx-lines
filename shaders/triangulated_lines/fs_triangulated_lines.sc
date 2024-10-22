$input v_uv, v_color
#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_length;

#define u_linelength    u_length.x
#define u_width         u_data.x
#define u_heigth        u_data.y
#define u_antialias     u_data.z
#define u_thickness     u_data.w

vec4 ScreenToClip(vec4 coordinate, float width, float height) {
    return vec4((2.0 * coordinate.x/ width) - 1, 
                (2.0 * coordinate.y/ height) - 1, 
                coordinate.z, 
                coordinate.w);
}

void main() {
	float d = 0;
  float width = u_thickness / 2.0 - u_antialias;
  vec4 color = v_color;

  if(v_uv.x < 0) {
    d = length(v_uv) - width;

  } else if(v_uv.x >= u_linelength) {

    d = length(v_uv - vec4(u_linelength, 0.0, 0.0, 0.0)) - width;
    
  } else {
    d = abs(v_uv.y) - width;
  }

	if(d < 0) {
      d = 1.0;
	} else {
      d /= u_antialias;
		  d = exp(-d * d); 
	}
	
  gl_FragColor = vec4(color.xyz, d);
}
