#version 330 core

out vec4 r_color;

in vec2 o_tex_coord;

uniform sampler2D u_texture;

void main() {
	r_color = texture(u_texture, o_tex_coord);
}