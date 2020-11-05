#version 330 core

// ================================================================================
// in & out

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out O_VS {
	vec2 tex_coord;
    vec3 pos_from_cam;
} o_vs;

uniform mat4 u_inv_proj;

// ================================================================================

void main() {

    o_vs.tex_coord = a_tex_coord;
    vec4 t_pos = (u_inv_proj * vec4(a_pos.x, a_pos.y, 1.0, 1.0));
    o_vs.pos_from_cam = t_pos.xyz / t_pos.w;

    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0); 

}

// ================================================================================