#version 330 core

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

uniform sampler2D u_texture;

void main() {
	r_color = texture(u_texture, i_fs.tex_coord);
}

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