$input a_position
$output v_color, v_length, v_is_start_end, v_uv, v_normal

#include <bgfx_compute.sh>
#include "../../polylines.sh"

BUFFER_RO(pointsBuffer, vec4, 1);

#define get_float_value(pos) pointsBuffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)        vec4(get_float_value(((pos) * 7) + 0), get_float_value(((pos) * 7) + 1), get_float_value(((pos) * 7) + 2), 0)
#define color(pos)    get_float_value(((pos) * 7) + 3)
#define normal(pos)   vec3(get_float_value(((pos) * 7) + 4), get_float_value(((pos) * 7) + 5), get_float_value(((pos) * 7) + 6))

uniform vec4 u_data;

#define a_uv                    a_position 

void main() {
    vec4 u_general_color = uintToVec4FloatColor(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_join_color = floatBitsToUint(u_data.w);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    float u_miter_limit  = float(thickness_antialias_border_miterlimit               & uint(0xFF));
    
    float u_leftCap      = float((caps_join_color >> uint(6)) & uint(0x3));
    float u_rigthCap     = float((caps_join_color >> uint(4)) & uint(0x3));
    float u_join         = float((caps_join_color >> uint(2)) & uint(0x3));
    float u_color_to_use = float((caps_join_color)            & uint(0x3));

    uint index = gl_InstanceID + 1;
    
    vec4 prev = p(index - 1);
    vec4 curr = p(index);
    vec4 next = p(index + 1);


    vec4 prev_px = calculatePointWithMVP(prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(next, u_screenWidth, u_screenHeigth);

    v_color = uintToVec4FloatColor(floatBitsToUint(color(index)));
    v_normal = normal(index);
    v_uv = vec4(0, 0, 0, 0);
    v_length = 0;

    bool is_start = false;
    bool is_end = false;
    v_is_start_end = 1;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
}