#version 330 core

out vec4 r_color;

in vec2 o_tex_coord;

uniform sampler2D u_texture_left;
uniform sampler2D u_texture_right;

vec4 vr_mix() {
	vec4 l_color = texture(u_texture_left, o_tex_coord);
	vec4 r_color = texture(u_texture_right, o_tex_coord);

	return vec4(l_color.b, l_color.g, r_color.g, 1);
}

void main() {
	r_color = vr_mix();
}