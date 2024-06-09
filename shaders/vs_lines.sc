$input a_position
$output v_color0

#include <bgfx_shader.sh>

uniform vec4 u_resolution;
uniform vec4 u_antialias;
uniform vec4 u_thickness;

uniform vec4 u_p0;
uniform vec4 u_p1;

void main() {
    vec4 resolution = normalize(u_resolution);
    float t = u_thickness.x / 2.0 + u_antialias.x;
    float l = length(u_p1 - u_p0);

    float u = 2.0 * a_position.x - 1.0;
    float v = 2.0 * a_position.y - 1.0;

    vec4 T = normalize(u_p1 - u_p0);
    vec4 O = vec4(-T.y , T.x, 0.0, 0.0);

    vec4 p = u_p0 + (a_position.x * T * l) + (u * T * t) + (v * O * t);

    gl_Position = vec4(p.xyz, 1.0);
    v_color0 = vec4(1.0, 1.0, 0.0, 1.0);
}