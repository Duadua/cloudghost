#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
 
out vec3 o_world_pos;
out vec3 o_normal;
out vec2 o_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
 
void main(){
  
    o_world_pos = vec3(u_model * vec4(a_pos, 1.0));
    o_normal = mat3(transpose(inverse(u_model))) * a_normal;  // �޸� ���ȱ����� �Է��ߵ��ƻ� -- ���߾���
    o_tex_coord = a_tex_coord;

    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);

}

/*
  a_* -- layout in
  o_* -- out
  u_* -- uniform
*/
