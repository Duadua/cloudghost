#version 330 core

out vec4 r_color;

in vec2 o_tex_coord;

uniform sampler2D u_texture_left;
uniform sampler2D u_texture_right;

vec4 vr_mix() {
	vec4 l_color = texture(u_texture_left, o_tex_coord);
	vec4 r_color = texture(u_texture_right, o_tex_coord);

//				0,			l_color.g,	l_color.b,	1
//				r_color.r,	r_color.g,	0,			1
	return vec4(r_color.g, l_color.g, 0, 1);
}

void main() {
	r_color = vr_mix();
}