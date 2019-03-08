#version 330 core

out vec4 r_color;

uniform float u_near;
uniform float u_far;

float origin_depth() { return gl_FragCoord.z; }
float linearize_depth(float o_depth) {
	float z = o_depth * 2.0 - 1.0;	// NDC
	float res;
	return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));	
																	// z = (a*zp + b) / zp
																	// z = a + b / zp
																	// a = (f + n) / (f - n)
																	// b = (-2.0*n*f) / (f - n)
																	// 投影矩阵的推导逆过程
	return res;
}

void main() {

	float depth = linearize_depth(gl_FragCoord.z) / u_far;
	r_color = vec4(vec3(depth), 1.0);
}

/**
*	depth test 
*	测试通过 -- 片段被绘制，且更新深度缓冲区的值
*	测试失败 -- 片段丢弃
*	深度测试在 frag_shader 执行完之后执行
*	深度测试在 模板测试 执行完之后执行 -- 模板测试见 solid_color.frag
*	glEnable(GL_DEPTH_TEST);	// 打开深度测试	
*	glDisable(GL_DEPTH_TEST);	// 关闭深度测试	
*	glDepthMask(GL_FALSE);		// 深度掩码 -- 置为 GL_FALSE 禁止写入深度缓冲 -- 默认为 GL_TRUE
*	glDepthFunc(GL_LESS);		// 深度测试函数 -- 决定什么时候通过 -- 默认 GL_LESS -- 小于当前缓冲区值时不丢弃
*/