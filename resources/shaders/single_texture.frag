#version 330 core

// ================================================================================
// const

const float pi = acos(-1.0);

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	mat3 tbn;
} i_fs;

in O_APos {
    vec3 world_pos;             // 没有任何变换的坐标
} i_apos;

// ================================================================================
// uniform

uniform sampler2D u_texture;
uniform bool		u_b_sphere_tex_coord;	// 是否使用球形的纹理坐标 -- 重新计算

// ================================================================================
// pre cac

vec2 t_tex_coord;

vec2 cac_sphere_tex_coord(vec3 v);              // 球形uv

void pre_main();

// ================================================================================

void main() {

  // pre cac
    pre_main();

	r_color = texture(u_texture, t_tex_coord);
}

// ================================================================================
// pre cac

vec2 cac_sphere_tex_coord(vec3 v) {
	vec2 res = vec2(atan(v.z, v.x), asin(v.y)); // ([-pi ,, pi], [-pi/2.0 ,, pi/2.0])

	// 映射到 [0 ,, 1]
	//res.x = res.x / (2.0 * pi) + 0.5;
	//res.y = res.y / pi + 0.5;

	res = res / vec2(2.0 * pi, pi) + 0.5;

	return res;

}

void pre_main() {

	t_tex_coord = i_fs.tex_coord;
    if(u_b_sphere_tex_coord) { t_tex_coord = cac_sphere_tex_coord(normalize(i_apos.world_pos)); }

}

// ================================================================================

/**
*	blend -- 混合
*	glEnable(GL_BLEND);		// 开启混合
*	glBlendFunc(GLenum sfactor, GLenum dfactor);	// 混合函数
*	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*													// -- sfactor -- 源因子		-- 当前片段颜色
*													// -- dfactor -- 目标因子	-- 之前颜色缓冲中的片段颜色
* 绘制有不透明物体的场景
*	-- 先绘制所有不透明物体
*	-- 对所有半透明物体按 距摄像机的远近排序
*	-- 按顺序绘制半透明物体 -- 由远及近
*/

// ================================================================================