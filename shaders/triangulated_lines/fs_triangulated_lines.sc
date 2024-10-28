$input v_uv, v_color
#include <bgfx_shader.sh>

uniform vec4 u_data;
uniform vec4 u_length;

#define u_linelength    u_length.x
#define u_width         u_data.x
#define u_heigth        u_data.y
#define u_antialias     u_data.z
#define u_thickness     u_data.w

void main() {
	float d = 0;
  float line_width = u_thickness / 2.0 - u_antialias;
  float line_lenght_px = u_linelength * u_width / 2;
  
  vec4 color = v_color;

  if(v_uv.x < 0) {
    d = length(v_uv) - line_width;

  } else if(v_uv.x >= line_lenght_px) {

    d = length(v_uv - vec4(line_lenght_px, 0.0, 0.0, 0.0)) - line_width;
    
  } else {
    d = abs(v_uv.y) - line_width;
  }

	if(d < 0) {
      d = 1.0;
	} else {
      d /= u_antialias;
		  d = exp(-d * d); 
	}
	
  gl_FragColor = vec4(color.xyz, d * color.w);

  #if 0
    if(gl_PrimitiveID % 2 == 0) {
      gl_FragColor = vec4(0.0, 1.0, 0.0, d);
    } else {
      gl_FragColor = vec4(1.0, 0.0, 1.0, d);
    }
  #endif
}
