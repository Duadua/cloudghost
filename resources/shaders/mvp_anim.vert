#version 330 core

// ================================================================================
// const 

// bone num
const int MAX_BONES = 100; 

// ================================================================================
// in & out

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;			// 副切线
layout (location = 5) in ivec4 a_bone_id;
layout (location = 6) in vec4 a_bone_weight;
 
out O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	mat3 tbn;
} o_vs;

out O_APos {
    vec3 world_pos;             // 没有任何变换的坐标
} o_apos;

out O_NormalProj {
	vec3 normal;
} o_np;

// ================================================================================
// uniform

// mvp -- uniform block
layout (std140) uniform Matrices {
	mat4 u_view;
	mat4 u_projection;
};
uniform mat4 u_model;
// bones
uniform mat4 u_bones[MAX_BONES];

// ================================================================================
// pre cac 

mat4 t_bone_mat = mat4(1.0);
mat4 t_model = mat4(1.0);
mat3 t_normal_mat = mat3(1.0);

mat3 tbn_cac();

void pre_main();

// ================================================================================

void main() {

	// pre_cac
	pre_main();

	// out o_vs
	o_apos.world_pos = a_pos;
    o_vs.world_pos = vec3(t_model * vec4(a_pos, 1.0));

    o_vs.normal = normalize(t_normal_mat * a_normal);
    o_vs.tex_coord = a_tex_coord;

	o_vs.tbn = tbn_cac();

	// out o_np
	vec3 t_normal_proj = mat3(transpose(inverse(u_view * t_model))) * a_normal;
	o_np.normal = normalize(vec3(u_projection * vec4(t_normal_proj, 0.0)));		// 几何着色器在 裁剪空间计算

    gl_Position = u_projection * u_view * t_model * vec4(a_pos, 1.0);

}

// ================================================================================
// pre cac

mat3 tbn_cac() {
	// gen tbn
	vec3 _n = normalize(t_normal_mat * a_normal);
	vec3 _t = normalize(t_normal_mat * a_tangent); _t = normalize(_t - dot(_t, _n) * _n);	// gram_schmidt 正交化
    vec3 _b = normalize(t_normal_mat * a_bitangent);
    //vec3 _b = -normalize(cross(_n, _t));
	return mat3(_t, _b, _n);
}

void pre_main() {
	// pre cac bone mat
	if(a_bone_weight[0] > 0.0f) t_bone_mat  = u_bones[a_bone_id[0]] * a_bone_weight[0];
	if(a_bone_weight[1] > 0.0f) t_bone_mat += u_bones[a_bone_id[1]] * a_bone_weight[1];
	if(a_bone_weight[2] > 0.0f) t_bone_mat += u_bones[a_bone_id[2]] * a_bone_weight[2];
	if(a_bone_weight[3] > 0.0f) t_bone_mat += u_bones[a_bone_id[3]] * a_bone_weight[3];

	t_model = u_model * t_bone_mat;

	t_normal_mat = transpose(inverse(mat3(t_model)));			// 法线矩阵 -- 变换法线到 world space 同步修正缩放影响

}

// ================================================================================