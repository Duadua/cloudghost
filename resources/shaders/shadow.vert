#version 330 core
#define MAX_BONES 100

layout (location = 0) in vec3 a_pos;
layout (location = 5) in ivec4 a_bone_id;
layout (location = 6) in vec4 a_bone_weight;
 
uniform mat4 u_light_proj_view;
uniform mat4 u_model;
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

    gl_Position = u_light_proj_view * t_model * vec4(a_pos, 1.0f);
}


