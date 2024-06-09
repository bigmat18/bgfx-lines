$input a_position
$output v_color0

#include <bgfx_shader.sh>

uniform vec4 u_resolution;
uniform vec4 u_antialias;
uniform vec4 u_thickness;

uniform vec4 u_p0;
uniform vec4 u_p1;

void main() {
    float t = u_thickness.x / 2.0 + u_antialias.x;
    float l = length(u_p1 - u_p0);

    float u = 2.0 * a_position.x - 1.0;
    float v = 2.0 * a_position.y - 1.0;

    // vec2 T = normalize(u_p1 - u_p0);
    // vec2 O = vec2(-T.y , T.x);
    // vec2 p = u_p0 + (a_position.x * T * l) + (u * T * t) + (v * O * t);
    // gl_Position = vec4(2.0 * p/u_resolution.x - 1.0, 2.0 * p/u_resolution.y - 1.0, 0.0, 1.0);

    gl_Position = vec4(a_position, 1.0);
    v_color0 = vec4(normalize(u_resolution.xyz), 1.0);
}