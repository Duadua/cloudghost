#version 330 core

out vec4 r_color;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
} i_fs;


void main() {
	r_color = vec4(1.0);
}
