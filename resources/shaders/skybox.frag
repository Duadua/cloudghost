#version 330 core

out vec4 r_color;

in O_VS {
	vec3 tex_coord;
} i_fs;

uniform samplerCube	u_texture;

void main() {
	r_color = texture(u_texture, i_fs.tex_coord);
}



