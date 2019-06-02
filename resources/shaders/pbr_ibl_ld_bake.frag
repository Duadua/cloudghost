#version 330 core

// ================================================================================
// const 

const float pi = acos(-1.0);

const uint sampler_num = 1024u;	// 采样数

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec3 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform samplerCube	u_texture;
uniform float		u_roughness;							// 粗糙度

// ================================================================================
// pbr

// b d f
float brdf_d_tr_ggx(float n_o_h, float roughness);

// ibl_specular helper
float hammersley_radical_inverse(uint bits);
vec2  hammersley(uint i, uint sampler_num);								// hammersley 采样 -- 当前采样id 总采样数
vec3  importance_sampling_ggx(vec2 uv, float roughness, vec3 n); 		// 重要性采样 -- 均匀2D坐标 粗糙度 法线 -- 获得中间向量 h
float pbr_ibl_lod_get(float pdf, float w, float h, float sampler_num);	// 获得 lod -- pdf 宽 高 总采样数

// 积分
vec3 ibl_ld_integral(vec3 n, float roughness);				// LD项积分 -- 得 CubeMap -- 参数: 法线

// ================================================================================
// pre cac

vec3 t_normal;

void pre_main();

// ================================================================================

void main() {

	// pre_cac
	pre_main();

	vec3 t_color = ibl_ld_integral(t_normal, u_roughness);

	r_color = vec4(t_color, 1.0);

}

// ================================================================================
// pre cac

void pre_main() {

	// 切线空间向量基
	t_normal = normalize(i_fs.tex_coord);
}

// ================================================================================
// pbr

// d g f
float brdf_d_tr_ggx(float n_o_h, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;

	float x = a2;
	float y = (n_o_h*n_o_h * (a2-1.0) + 1.0);
	return x / max(pi * y * y, 0.001);
}

// ibl_specular helper
float hammersley_radical_inverse(uint bits) {
	bits = ( bits << 16u ) | ( bits >> 16u );
    bits = ( ( bits & 0x55555555u ) << 1u ) | ( ( bits & 0xAAAAAAAAu ) >> 1u );
    bits = ( ( bits & 0x33333333u ) << 2u ) | ( ( bits & 0xCCCCCCCCu ) >> 2u );
    bits = ( ( bits & 0x0F0F0F0Fu ) << 4u ) | ( ( bits & 0xF0F0F0F0u ) >> 4u );
    bits = ( ( bits & 0x00FF00FFu ) << 8u ) | ( ( bits & 0xFF00FF00u ) >> 8u );
    return float( bits ) * 2.3283064365386963e-10f;		// 2^(-32)
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
float pbr_ibl_lod_get(float pdf, float w, float h, float sampler_num) {
	float a = 4.0 * pi / (6.0 * w * h);
	float b = 1.0 / (pdf * sampler_num);

	if(pdf < 0.001) return 0.0;

	return max(0.5 * log2(b / a), 0.0);
}

// 积分
vec3 ibl_ld_integral(vec3 n, float roughness) {
	vec3 res = vec3(0.0);

	vec3 v = n;			// 假设 view = normal
	vec3 r = n;			// 假设 reflect = normal

	float tot_weight = 0.0;
	for(uint i = 0u; i < sampler_num; ++i) {
		vec2 uv = hammersley(i, sampler_num);							// hammersley 采样
		vec3 h = importance_sampling_ggx(uv, roughness, n); 			// 重要性采样
		vec3 l = normalize(2.0 * dot(v, h) * h - v);					// 从 v 和 h 逆算 l -- 光源方向

		float n_o_v = max(dot(n, v), 0.0);							
		float n_o_l = max(dot(n, l), 0.0);
		float n_o_h = max(dot(n, h), 0.0);
		float h_o_v = max(dot(h, v), 0.0);

		if(n_o_l > 0.0) {
			float d = brdf_d_tr_ggx(n_o_h, roughness);					// 法线分布函数

			float pdf = d * n_o_h / (4.0 * h_o_v);						// 计算 pdf
			float lod = pbr_ibl_lod_get(pdf, 512, 512, sampler_num);	// 获得 lod

			res += texture(u_texture, l).rgb * n_o_l;
			//res += textureLod(u_texture, l, lod).rgb * n_o_l;
			tot_weight += n_o_l;
		}

	}

	res /= tot_weight;

	return res;
}

