$input a_position
$output v_color

#include <bgfx_compute.sh>
#include "../../polylines.sh"

BUFFER_RO(pointsBuffer, float, 1);
#define p(pos)    vec4(pointsBuffer[0 + ((pos) * 3)], pointsBuffer[1 + ((pos) * 3)], pointsBuffer[2 + ((pos) * 3)], 0.0)

uniform vec4 u_data;
uniform vec4 u_color;
uniform vec4 u_IndirectData;

#define a_uv              a_position 
#define u_width           u_data.x
#define u_heigth          u_data.y
#define u_miter_limit     u_data.z
#define u_thickness       u_data.w

void main() {
    vec4 a_prev = p(gl_InstanceID - sign(gl_InstanceID));
    vec4 a_curr = p(gl_InstanceID);
    vec4 a_next = p(gl_InstanceID + sign(u_IndirectData.x - gl_InstanceID));
    vec4 a_nextnext = p(gl_InstanceID + sign(u_IndirectData.x - gl_InstanceID) + sign(u_IndirectData.x - 1 - gl_InstanceID));

    vec4 prev = ((1 - a_uv.x) * a_prev) + (a_uv.x * a_curr);
    vec4 curr = ((1 - a_uv.x) * a_curr) + (a_uv.x * a_next);
    vec4 next = ((1 - a_uv.x) * a_next) + (a_uv.x * a_nextnext);

    v_color = u_color;
    gl_Position = calculatePolylines(prev, curr, next, a_uv, u_thickness, u_miter_limit, u_width, u_heigth);
}
