#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tex_coord;
 
out vec3 o_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
void main(){
  gl_Position = projection * view * model * vec4(a_pos, 1.0f);
  o_color = vec3(1.0f, 0.0f, 0.0f);

}
