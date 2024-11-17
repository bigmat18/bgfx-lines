$input a_position
$output v_color

#include <bgfx_shader.sh>
SAMPLER2D(s_Texture,  0);

void main() {
    vec4 p0 = texelFetch(u_instanceTexture, ivec2(gl_InstanceID, 0), 0);
    vec4 p1 = texelFetch(u_instanceTexture, ivec2(gl_InstanceID + 0, 0), 0);
    vec4 color = texelFetch(u_instanceTexture, ivec2(gl_InstanceID + 1, 0), 0);

    v_color = color;
    gl_Position = vec4(a_position.xy, 0.0, 1.0);
}
