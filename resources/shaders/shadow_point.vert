#version 330 core

// ================================================================================
// const 

// bone num
const int MAX_BONES = 100; 

// ================================================================================
// in & out

layout (location = 0) in vec3 a_pos;
layout (location = 5) in ivec4 a_bone_id;
layout (location = 6) in vec4 a_bone_weight;
 
// ================================================================================
// uniform

uniform mat4 u_model;
uniform mat4 u_bones[MAX_BONES];

// ================================================================================
// pre cac

mat4 t_bone_mat	 = mat4(1.0);
mat4 t_model = mat4(1.0);

void pre_main();

// ================================================================================

void main() {

	// pre cac 
	pre_main();

    gl_Position = t_model * vec4(a_pos, 1.0f);
}

// ================================================================================
// pre cac

void pre_main() {
	if(a_bone_weight[0] > 0.0f) t_bone_mat  = u_bones[a_bone_id[0]] * a_bone_weight[0];
	if(a_bone_weight[1] > 0.0f) t_bone_mat += u_bones[a_bone_id[1]] * a_bone_weight[1];
	if(a_bone_weight[2] > 0.0f) t_bone_mat += u_bones[a_bone_id[2]] * a_bone_weight[2];
	if(a_bone_weight[3] > 0.0f) t_bone_mat += u_bones[a_bone_id[3]] * a_bone_weight[3];

	t_model = u_model * t_bone_mat;
}

// ================================================================================
