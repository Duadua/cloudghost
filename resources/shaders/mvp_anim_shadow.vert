#version 330 core
#define MAX_BONES 100

// light num 
const int max_direct_light_num	= 1;
const int max_point_light_num	= 4;
const int max_spot_light_num	= 4;
const int max_sky_light_num	    = 1;

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 5) in ivec4 a_bone_id;
layout (location = 6) in vec4 a_bone_weight;
 
out O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	vec4 direct_light_pv_pos[max_direct_light_num];
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

uniform mat4		u_direct_light_proj_view[max_direct_light_num];
uniform int         u_direct_light_num;								// default is 0
uniform int         u_point_light_num;
uniform int         u_spot_light_num;
uniform int         u_sky_light_num;

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
    o_vs.tex_coord = a_tex_coord;

	o_np.normal = normalize(vec3(u_projection * vec4(o_vs.normal, 0.0)));		// 几何着色器在 裁剪空间计算

	for(int i = 0; i < u_direct_light_num; ++i) {
		o_vs.direct_light_pv_pos[i] = u_direct_light_proj_view[i] * t_model * vec4(a_pos, 1.0f);
		//if(o_vs.direct_light_pv_pos[i].xyz == o_vs.world_pos) { o_vs.direct_light_pv_pos[i].z = 0.0; }
	}

    gl_Position = u_projection * u_view * t_model * vec4(a_pos, 1.0f);

	// gl_PointSize = gl_Position.z;

}
