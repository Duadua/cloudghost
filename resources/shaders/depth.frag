#version 330 core

out vec4 r_color;

uniform float u_near;
uniform float u_far;

float origin_depth() { return gl_FragCoord.z; }
float linearize_depth(float o_depth) {
//	float t_far = u_far * 0.01;
//	float t_near = u_near * 0.01;
	float t_far = u_far;
	float t_near = u_near;

	float z = o_depth * 2.0 - 1.0;	// NDC

	float res = (2.0 * t_near * t_far) / (t_far + t_near - z * (t_far - t_near));	
																	// z = (a*zp + b) / zp
																	// z = a + b / zp
																	// a = (f + n) / (f - n)
																	// b = (-2.0*n*f) / (f - n)
																	// 投影矩阵的推导逆过程
																	// 求得的 zp 为 [near, far] 上的线性深度
	return (res - t_near) / (min(100.0, t_far) - t_near);
}

void main() {

	float depth = linearize_depth(gl_FragCoord.z);
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

/**
*	内置变量
*	gl_FragCoord -- (x, y) -- 片断在窗口的坐标 -- 其范围为 glviewport() 设定的窗口大小 -- 而非 [0.0 ,, 1.0]
*				 -- z -- 片断的深度值 -- z = [0.0 ,, 1.0] -- 只读
*					  -- 并非线性深度 -- 原线性深度值经投影变换得到的 NDC(标准化设备[-1, 1]) 深度值再映射到 [0, 1] 得到
*	gl_FragDepth -- 片断的深度值 -- 可写 -- 用来设置片断的深度值 - 传入一个 [0.0 ,, 1.0] 的数 -- 默认为 gl_FragCoord.z
*				 -- 会关闭所有的提前深度测试 (Early Depth Test) -- 420版本可使用 深度条件(Depth Condition) 修改其值
*	gl_FrontFacing -- 是否为正向面 -- bool值	
*/






