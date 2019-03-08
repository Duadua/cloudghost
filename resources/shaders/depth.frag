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