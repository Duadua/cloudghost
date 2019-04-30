#version 330 core

// ================================================================================
// in & out

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out O_VS {
	vec2 tex_coord;
} o_vs;

// ================================================================================

void main() {

    o_vs.tex_coord = a_tex_coord;

    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0); 

}

// ================================================================================