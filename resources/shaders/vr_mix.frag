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

	float t_l = (l_color.g + l_color.b + l_color.r) / 3.0;
	float t_r = (r_color.r + r_color.g + r_color.b) / 3.0;
	return vec4(t_l*1.3, t_r*0.3, t_r*0.7, 1.0);
}

void main() {
	r_color = vr_mix();
}