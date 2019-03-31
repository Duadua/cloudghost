#version 330 core
#define MAX_BONES 100

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 5) in ivec4 a_bone_id;
layout (location = 6) in vec4 a_bone_weight;
 
out vec3 o_world_pos;
out vec3 o_normal;
out vec2 o_tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_bones[MAX_BONES];

void main() {
	mat4 t_bone_mat	 = mat4(1.0f);
	if(a_bone_weight[0] > 0.0f) t_bone_mat  = u_bones[a_bone_id[0]] * a_bone_weight[0];
	if(a_bone_weight[1] > 0.0f) t_bone_mat += u_bones[a_bone_id[1]] * a_bone_weight[1];
	if(a_bone_weight[2] > 0.0f) t_bone_mat += u_bones[a_bone_id[2]] * a_bone_weight[2];
	if(a_bone_weight[3] > 0.0f) t_bone_mat += u_bones[a_bone_id[3]] * a_bone_weight[3];
	//t_bone_mat  = u_bones[a_bone_id[0]] * a_bone_weight[0];
	//t_bone_mat += u_bones[a_bone_id[1]] * a_bone_weight[1];
	//t_bone_mat += u_bones[a_bone_id[2]] * a_bone_weight[2];
	//t_bone_mat += u_bones[a_bone_id[3]] * a_bone_weight[3];

	mat4 t_model = u_model * t_bone_mat;

    o_world_pos = vec3(t_model * vec4(a_pos, 1.0));
    o_normal = mat3(transpose(inverse(t_model))) * a_normal;  // 法线矩阵 -- 变换法线到 world space 同步修正缩放影响
    o_tex_coord = a_tex_coord;

    gl_Position = u_projection * u_view * t_model * vec4(a_pos, 1.0f);

	// gl_PointSize = gl_Position.z;

}

/*
  a_* -- layout in
  o_* -- out
  u_* -- uniform
*/

/*
	内建变量
	gl_Position -- 顶点位置 -- 决定顶点的最终位置
	gl_PointSize -- 顶点大小 -- 需要开启 GL_PROGRAM_POINT_SIZE
	gl_VertexID -- 顶点索引值
*/