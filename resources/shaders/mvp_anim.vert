#version 330 core
#define MAX_BONES 100

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec3 a_tangent;			// 切线
layout (location = 4) in vec3 a_bitangent;			// 副切线
layout (location = 5) in ivec4 a_bone_id;
layout (location = 6) in vec4 a_bone_weight;
 
out O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	mat3 tbn;
} o_vs;

out O_NormalProj {
	vec3 normal;
} o_np;

layout (std140) uniform Matrices {
	mat4 u_view;
	mat4 u_projection;
};
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

    o_vs.world_pos = vec3(t_model * vec4(a_pos, 1.0));
    o_vs.normal = mat3(transpose(inverse(t_model))) * a_normal;  // 法线矩阵 -- 变换法线到 world space 同步修正缩放影响
    //o_vs.tangent = mat3(transpose(inverse(t_model))) * a_tangent;  
    o_vs.tex_coord = a_tex_coord;

	vec3 t_normal_proj = mat3(transpose(inverse(u_view * t_model))) * a_normal;
	o_np.normal = normalize(vec3(u_projection * vec4(t_normal_proj, 0.0)));		// 几何着色器在 裁剪空间计算

    gl_Position = u_projection * u_view * t_model * vec4(a_pos, 1.0f);

	// gl_PointSize = gl_Position.z;

}

/**
*	a_* -- layout in
*	o_* -- out
*	u_* -- uniform
*/

/**
*	内建变量
*	gl_Position -- 顶点位置 -- 决定顶点的最终位置
*	gl_PointSize -- 顶点大小 -- 需要开启 GL_PROGRAM_POINT_SIZE
*	gl_VertexID -- 顶点索引值
*/


