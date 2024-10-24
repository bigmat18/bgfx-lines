$input v_uv, v_color
#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_length;

#define u_linelength    u_length.x
#define u_width         u_data.x
#define u_heigth        u_data.y
#define u_antialias     u_data.z
#define u_thickness     u_data.w

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
	float d = 0;
  float width = u_thickness / 2.0 - u_antialias;
  vec4 color = v_color;

  vec4 uv = v_uv;

  if(uv.x < 0) {
    d = length(uv) - width;

  } else if(uv.x >= u_linelength) {

    d = length(uv - vec4(u_linelength, 0.0, 0.0, 0.0)) - width;
    
  } else {
    d = abs(uv.y) - width;
  }

	if(d < 0) {
      d = 1.0;
	} else {
      d /= u_antialias;
		  d = exp(-d * d); 
	}
	
  gl_FragColor = vec4(color.xyz, d * color.w);
  gl_FragColor = color;

  #if 1
    if(gl_PrimitiveID % 2 == 0) {
      gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    } else {
      gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
  #endif
}
