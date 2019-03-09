#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec2 o_tex_coord;

void main() {

    o_tex_coord = a_tex_coord;

    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0); 

}