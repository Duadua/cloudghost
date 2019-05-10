#version 330 core

// ================================================================================
// const 

const float pi = acos(-1.0);

const uint sampler_num = 1024u;	// 采样数

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

// ================================================================================
// pbr 

// d g f
float brdf_g_k_ibl(float roughness);
float brdf_g_schlick_ggx(float n_o_v, float k);
float brdf_g_smith(float n_o_v, float n_o_l, float k);

// ibl_specular helper
float hammersley_radical_inverse(uint bits);
vec2  hammersley(uint i, uint sampler_num);								// hammersley 采样 -- 当前采样id 总采样数
vec3  importance_sampling_ggx(vec2 uv, float roughness, vec3 n); 		// 重要性采样 -- 均匀2D坐标 粗糙度 法线 -- 获得中间向量 h

// ibl dgf interal 积分
vec2 ibl_dgf_integral(float n_o_v, float roughness);

// ================================================================================

void main() {

	vec2 t_color = vec2(0.0, 0.0);
	
	t_color = ibl_dgf_integral(i_fs.tex_coord.x, i_fs.tex_coord.y);
	r_color = vec4(t_color, 0.0, 1.0);

}

// ================================================================================
// pbr

// d g f
float brdf_g_k_ibl(float roughness) { return roughness*roughness / 2.0; }
float brdf_g_schlick_ggx(float n_o_v, float k) {
	float x = n_o_v;
	float y = n_o_v * (1.0 - k) + k;
	return x / y;
}
float brdf_g_smith(float n_o_v, float n_o_l, float k) {
	float g_v = brdf_g_schlick_ggx(n_o_v, k);
	float g_l = brdf_g_schlick_ggx(n_o_l, k);

	return g_v * g_l;
}

// ibl_specular helper
float hammersley_radical_inverse(uint bits) {
	bits = ( bits << 16u ) | ( bits >> 16u );
    bits = ( ( bits & 0x55555555u ) << 1u ) | ( ( bits & 0xAAAAAAAAu ) >> 1u );
    bits = ( ( bits & 0x33333333u ) << 2u ) | ( ( bits & 0xCCCCCCCCu ) >> 2u );
    bits = ( ( bits & 0x0F0F0F0Fu ) << 4u ) | ( ( bits & 0xF0F0F0F0u ) >> 4u );
    bits = ( ( bits & 0x00FF00FFu ) << 8u ) | ( ( bits & 0xFF00FF00u ) >> 8u );
    return float( bits ) * 2.3283064365386963e-10f;
}
vec2 hammersley(uint i, uint sampler_num) {
	return vec2(float(i) / float(sampler_num), hammersley_radical_inverse(i));
}
vec3 importance_sampling_ggx(vec2 uv, float roughness, vec3 n) {
	float a = roughness * roughness;

	float phi = 2.0 * pi * uv.x;
	float cos_theta = sqrt((1.0 - uv.y) / (1.0 + (a * a - 1.0) * uv.y));
	float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	vec3 h = vec3(
		sin_theta * cos(phi),
		sin_theta * sin(phi),
		cos_theta
	);	// 此时为切换空间的 h

	// 获得 tbn 空间的 h
	vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 right = normalize(cross(up, n));
	up = normalize(cross(n, right));

	return normalize(right * h.x + up * h.y + n * h.z);

}

// ibl dgf interal 积分
vec2 ibl_dgf_integral(float n_o_v, float roughness) {

	float scale = 0.0;
	float bias  = 0.0;

	vec3 n = vec3(0.0, 0.0, 1.0);
	vec3 v = vec3(
		sqrt(1.0 - n_o_v * n_o_v),
		0.0,
		n_o_v
	);

	for(uint i = 0u; i < sampler_num; ++i) {
		vec2 uv = hammersley(i, sampler_num);							// hammersley 采样
		vec3 h = importance_sampling_ggx(uv, roughness, n); 			// 重要性采样
		vec3 l = normalize(2.0 * dot(v, h) * h - v);					// 从 v 和 h 逆算 l -- 光源方向

		//float n_o_v = max(dot(n, v), 0.0);							// 参数1
		float n_o_l = max(dot(n, l), 0.0);
		float n_o_h = max(dot(n, h), 0.0);
		float h_o_v = max(dot(h, v), 0.0);

		if(n_o_l > 0.0) {
			float k = brdf_g_k_ibl(roughness);							// 根据直接光源/IBL 选择相应方程

			float g = brdf_g_smith(n_o_v, n_o_l, k);					// 几何函数
			float f = pow(1.0 - h_o_v, 5.0);							// 菲涅尔方程

			float g_vis = g * h_o_v / (n_o_h * n_o_v);

			scale += (1.0 - f) 	* g_vis;
			bias  += f 			* g_vis;

		}

	}

	scale /= float(sampler_num);
	bias  /= float(sampler_num);

	return vec2(scale, bias);
}

// ================================================================================











