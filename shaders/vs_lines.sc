$input a_position
$output v_p, v_p0, v_p1, v_color

#include <bgfx_shader.sh>

uniform vec4 u_p0;
uniform vec4 u_p1;
uniform vec4 u_data;

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
    vec4 center = ScreenToClip((u_p0 + u_p1) / 2.0, u_width, u_heigth);
    vec4 viewPos = vec4(u_view[0][3], u_view[1][3], u_view[2][3], 1.0);

    float dotProduct = dot(normalize(center), normalize(viewPos));
    float cosTheta = dotProduct / (length(center) * length(viewPos));
    float angle = acos(cosTheta);

    mat4 rotationMtx = mat4(
        cos(angle),  0.0,  sin(angle), 0.0,
        0.0,         1.0,  0.0,        0.0,
        -sin(angle), 0.0,  cos(angle), 0.0,
        0.0,         0.0,  0.0,        1.0
    );
    
    v_color = u_color;

    float t = u_thickness / 2.0 + u_antialias;
    float l = length(u_p1 - u_p0);

    float u = 2.0 * a_position.x - 1.0;
    float v = 2.0 * a_position.y - 1.0;

    vec4 T = normalize(u_p1 - u_p0);
    vec4 O = vec4(-T.y , T.x, 0.0, 0.0);

    vec4 p = u_p0 + (a_position.x * T * l) 
                  + (u * T * t) 
                  + (v * O * t);

    p = ScreenToClip(p, u_width, u_heigth);
    vec4 rotateP = rotationMtx * p;

    gl_Position = mul(u_modelViewProj, vec4(rotateP.xyz, 1.0));

    T = vec4(1.0, 0.0, 0.0, 0.0);
    O = vec4(0.0, 1.0, 0.0, 0.0);
    p = (a_position.x * T * l) + 
        (u * T * t) + 
        (v * O * t);

    v_p = vec4(p.xyz, 1.0);
    v_p0 = vec4(0.0, 0.0, 0.0, 1.0);
    v_p1 = vec4(l, 0.0, 0.0, 1.0);
}