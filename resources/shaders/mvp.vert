#version 330 core

// ================================================================================
// const 

// ================================================================================
// in & out
 
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec3 a_bitangent;			// 副切线
 
out O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	mat3 tbn;
} o_vs;

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

// ================================================================================
// pre cac 

mat3 t_normal_mat = mat3(1.0);

mat3 tbn_cac();

void pre_main();

// ================================================================================

void main() {

	// pre cac
	pre_main();
	
    o_vs.world_pos = vec3(u_model * vec4(a_pos, 1.0));

    o_vs.normal = t_normal_mat * a_normal;  // 法线矩阵 -- 变换法线到 world space 同步修正缩放影响
    o_vs.tex_coord = a_tex_coord;

	o_vs.tbn = tbn_cac();

	// out o_np
	vec3 t_normal_proj = mat3(transpose(inverse(u_view * t_model))) * a_normal;
	o_np.normal = normalize(vec3(u_projection * vec4(t_normal_proj, 0.0)));		// 几何着色器在 裁剪空间计算

    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);

}

// ================================================================================
// pre cac

mat3 tbn_cac() {
	// gen tbn
	vec3 _n = normalize(t_normal_mat * a_normal);
	vec3 _t = normalize(t_normal_mat * a_tangent); _t = normalize(_t - dot(_t, _n) * _n);	// gram_schmidt 正交化
    //vec3 _b = normalize(t_normal_mat * a_bitangent);
    vec3 _b = normalize(cross(_n, _t));
	return mat3(_t, _b, _n);
}

void pre_main() {

	t_normal_mat = transpose(inverse(mat3(t_model)));			// 法线矩阵 -- 变换法线到 world space 同步修正缩放影响

}

// ================================================================================

/**
*	a_* -- layout in
*	o_* -- out
*	u_* -- uniform
*/

/**
*	接口块
*	out O_VS { ... } o_vs;
*	打包了发送到下一个着色器的数据
*	在下一个着色器接收需定义一个相同名称的 in 接口块
*	in O_VS { ... } i_fs; -- 块名(O_VS) 需一样, 但是实例名随意 
*/

/**
*	uniform 块布局
*	layout (std140) uniform xxx {};
*	std140 -- 特定的布局结构 -- 4字节对齐
*/

// ================================================================================