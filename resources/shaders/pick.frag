#version 330 core

out uvec3 r_color;

uniform uint u_object_id;
uniform uint u_component_id;

void main() {
	r_color = uvec3(u_object_id, u_component_id, gl_PrimitiveID);
	//r_color = uvec3(1.0, 0.0, 0.0, 1.0);
}