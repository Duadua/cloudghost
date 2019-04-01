#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;
 
out O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
} o_vs;

layout (std140) uniform Matrices {
	mat4 u_view;
	mat4 u_projection;
};
uniform mat4 u_model;

void main() {
	
    o_vs.world_pos = vec3(u_model * vec4(a_pos, 1.0));
    o_vs.normal = mat3(transpose(inverse(u_model))) * a_normal;  // 法线矩阵 -- 变换法线到 world space 同步修正缩放影响
    o_vs.tex_coord = a_tex_coord;

    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);

}

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