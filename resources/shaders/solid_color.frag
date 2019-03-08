#version 330 core

out vec4 r_color;

uniform vec4 u_solid_color;

void main() {
	r_color = u_solid_color;
}