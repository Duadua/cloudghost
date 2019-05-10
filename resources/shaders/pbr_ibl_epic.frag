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
// light 

// light const -- number
const int max_light_direct_num	= 1;
const int max_light_point_num	= 4;
const int max_light_spot_num	= 4;
const int max_light_sky_num	    = 1;

struct LightDirect {
    vec3 color;
    vec3 direction;

    float intensity;
};

// ================================================================================
// material

// pbr material
struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;

    sampler2D map_ka;       // 0                
    sampler2D map_kd;       // 1
    sampler2D map_ks;       // 2
    sampler2D map_normal;   // 3
                            // 4 -- 以后加视差贴图用
	bool has_map_ka;
	bool has_map_kd;
	bool has_map_ks;
    bool has_map_normal;

    float shininess;

	// pbr 部分
	vec3 albedo;			// 返照率 -- 只包含表面颜色 -- 类似 diffuse
	float metallic;			// 金属度
	float roughness;		// 粗糙度
	float ao;				// 环境遮蔽

	sampler2D map_albedo;	// 5
	sampler2D map_metallic;	// 6
	sampler2D map_roughness;// 7
	sampler2D map_ao;		// 8

	bool has_map_albedo;
	bool has_map_metallic;
	bool has_map_roughness;
	bool has_map_ao;

};

// ================================================================================
// uniform

// uniform light
uniform LightDirect u_light_directs[max_light_direct_num];
uniform int			u_light_direct_num;

// uniform material
uniform Material	u_material;
uniform bool		u_normal_map_b_use;     // 是否使用法线贴图
uniform bool		u_b_sphere_tex_coord;	// 是否使用球形的纹理坐标 -- 重新计算

uniform samplerCube u_ibl_diffuse_map; 		// 9
uniform samplerCube u_ibl_ld_map; 			// 10 - LD 项
uniform sampler2D	u_ibl_dgf_map; 			// 11 - DGF 项

// uniform for cac
uniform vec3 		u_view_pos;

uniform int 		u_max_reflection_lod;	// ld项 cubemap 的最大  lod 数

// ================================================================================
// pbr 

// d g f
float brdf_d_tr_ggx(float n_o_h, float roughness);

float brdf_g_k_direct(float roughness);
float brdf_g_k_ibl(float roughness);
float brdf_g_schlick_ggx(float n_o_v, float k);
float brdf_g_smith(float n_o_v, float n_o_l, float k);

vec3  brdf_f_f0(vec3 f0, vec3 albedo, float metallic);							// albedo -- 反射率 -- 可能来自贴图 -- f0 为基础反射率
vec3  brdf_f_fresnel_schlick(float h_o_v, vec3 f0);								// h 为中间向量 -- 可作为微平面的法线 -- 有效的微平面
vec3  brdf_f_fresnel_schlick_roughness(float n_o_v, vec3 f0, float roughness);	// 考虑 粗糙度的 fresnel

// brdf
vec3 brdf_cook_torrance(vec3 n, vec3 v, vec3 l, 
						vec3 c_diffuse, vec3 c_specular, 
						vec3 f0, float roughness, float metallic);

// 反射率方程
vec3 pbr_Lo(vec3 n, vec3 v, vec3 l,
			vec3 c_diffuse, vec3 c_specular, 
			vec3 f0, float roughness, float metallic,
			vec3 radiance);									// radiance -- 辐射度(入射)

// ibl
vec3 pbr_ibl_diffuse(	vec3 n, vec3 v, 
						vec3 albedo,
						vec3 f0, float roughness, float metallic,
						vec3 irradiance);

// ibl_specular 
vec3 pbr_ibl_specular_epic(	vec3 n, vec3 v,
							vec3 albedo,
							vec3 f0, float roughness, float metallic);

// ================================================================================
// light cac

float light_direct_att(LightDirect light_d);
vec3  light_direct_one(LightDirect light_d);
vec3  light_direct();

vec3  light_ambient(vec3 coe); // coe -- 系数

// ================================================================================
// pre cac

vec2 t_tex_coord;

vec3  t_c_ambient;
vec3  t_c_diffuse;
vec3  t_c_specular;
vec3  t_irradiance_diffuse;								// 反射率 -- 环境光反射率 -- IBL 用

float t_metallic;
float t_roughnes;
float t_ao;

vec3  t_view_dir;
vec3  t_normal;

vec3 normal_from_texture();                     // 法线贴图

vec2 cac_sphere_tex_coord(vec3 v);              // 球形uv

void pre_main(); 

// ================================================================================

void main() {
	pre_main();

	vec3 t_color = vec3(0.0, 0.0, 0.0);
	
	t_color += light_direct();

	t_color += light_ambient(vec3(1.0));

	r_color = vec4(t_color, 1.0);

	//r_color = vec4(vec3(t_ao), 1.0);

	//r_color = vec4(vec3(t_roughnes), 1.0);
}

// ================================================================================
// pre cac

vec3 normal_from_texture() {
    // 缩小镜面光范围
    //t_shininess *= 64;

    // 切线空间的法线 --- 从法线贴图中得 [-1.0 ,, 1.0]
    vec3 tangent_normal = texture(u_material.map_normal, t_tex_coord).rgb;
    tangent_normal = normalize(tangent_normal * 2.0 - 1.0);

    return normalize(i_fs.tbn * tangent_normal);
}

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

	t_c_ambient = u_material.ka;
	t_c_diffuse =  u_material.kd;			// 片段本身基础颜色
	t_c_specular = u_material.ks;
	vec3 t_albedo = u_material.albedo;

	t_metallic = u_material.metallic;
	t_roughnes = u_material.roughness;
	t_ao = u_material.ao;

	if(u_material.has_map_ka) { t_c_ambient = texture(u_material.map_ka, t_tex_coord).rgb; }
	if(u_material.has_map_kd) { t_c_diffuse = texture(u_material.map_kd, t_tex_coord).rgb; }
	if(u_material.has_map_ks) { t_c_specular = texture(u_material.map_ks, t_tex_coord).rgb; }
	if(u_material.has_map_albedo) { t_albedo = texture(u_material.map_albedo, t_tex_coord).rgb; }

	if(u_material.has_map_metallic) { t_metallic = texture(u_material.map_metallic, t_tex_coord).r; }
	if(u_material.has_map_roughness) { t_roughnes = texture(u_material.map_roughness, t_tex_coord).r; }
	if(u_material.has_map_ao) { t_ao = texture(u_material.map_ao, t_tex_coord).r; }


	// pbr 里修正颜色
	t_c_diffuse = t_albedo;
	t_c_specular = t_albedo;			 
	t_c_ambient = 1.0 * t_c_diffuse;
	t_c_ambient *= t_ao;

	t_view_dir = normalize(u_view_pos - i_fs.world_pos);

    if(u_normal_map_b_use && u_material.has_map_normal) { t_normal = normal_from_texture(); }
    else { t_normal = normalize(i_fs.normal); }

	vec3 R = reflect(-t_view_dir, t_normal);
	t_irradiance_diffuse = texture(u_ibl_diffuse_map, R).rgb;

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

float brdf_g_k_direct(float roughness) { return (roughness + 1.0)*(roughness + 1.0) / 8.0; }
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

vec3 brdf_f_f0(vec3 f0, vec3 albedo, float metallic) {
	return mix(f0, albedo, metallic);
	// return metallic * albedo + (1.0 - metallic) * f0;
}
vec3 brdf_f_fresnel_schlick(float h_o_v, vec3 f0) {
	return f0 + (1.0 - f0) * pow(1.0 - h_o_v, 5.0);
}
vec3 brdf_f_fresnel_schlick_roughness(float n_o_v, vec3 f0, float roughness) {
	vec3 t_s = max(vec3(1.0 - roughness), f0); 
	return f0 + (t_s - f0) * pow(1.0 - n_o_v, 5.0);
}

// brdf
vec3 brdf_cook_torrance(vec3 n, vec3 v, vec3 l, vec3 c_diffuse, vec3 c_specular, vec3 f0, float roughness, float metallic) {
	// pre cac
	vec3 h = normalize(v + l);							// 中间向量
	float n_o_v = max(dot(n, v), 0.0);
	float n_o_l = max(dot(n, l), 0.0);
	float n_o_h = max(dot(n, h), 0.0);
	float h_o_v = max(dot(h, v), 0.0);

	float k = brdf_g_k_direct(roughness);				// 根据直接光源/IBL 选择相应方程
	vec3 f90 = brdf_f_f0(f0, c_specular, metallic);		// 获取真正的 f0 -- 有金属性影响

	// get d g f
	float d = brdf_d_tr_ggx(n_o_h, roughness);			// 法线分布函数
	float g = brdf_g_smith(n_o_v, n_o_l, k);			// 几何函数
	vec3  f = brdf_f_fresnel_schlick(h_o_v, f90);		// 菲涅尔方程

	// get k_s and k_d
	vec3  k_s = f;										// 镜面反射系数 -- 等于菲涅尔方程的值
	vec3  k_d = (vec3(1.0) - k_s) * (1.0 - metallic);	// 漫反射系数 -- 考虑金属度

	// get diffuse and specular
	vec3 diffuse = c_diffuse / pi;						
	vec3 x = d * g * f;
	float y = max(4.0 * n_o_v * n_o_l, 0.001);
	vec3 speculr = x / y;

	return 	k_d * diffuse + /*k_s */ speculr;			// 不需要 k_s -- 已经有菲涅尔系数了
}

// 反射率方程 -- 最终结果
vec3 pbr_Lo(vec3 n, vec3 v, vec3 l, vec3 c_diffuse, vec3 c_specular, vec3 f0, float roughness, float metallic, vec3 radiance) {
	float n_o_l = max(dot(n, l), 0.0);
	vec3 brdf = brdf_cook_torrance(n, v, l, c_diffuse, c_specular, f0, roughness, metallic);

	return brdf * radiance * n_o_l;
}

// IBL -- diffuse
vec3 pbr_ibl_diffuse(vec3 n, vec3 v, vec3 albedo, vec3 f0, float roughness, float metallic, vec3 irradiance) {
	float n_o_v = max(dot(n, v), 0.0);

	vec3 f90 = brdf_f_f0(f0, albedo, metallic);								// 获取真正的 f0 -- 有金属性影响
	vec3  f = brdf_f_fresnel_schlick_roughness(n_o_v, f90, roughness);		// 菲涅尔方程

	// get k_s and k_d
	vec3  k_s = f;										// 镜面反射系数 -- 等于菲涅尔方程的值
	vec3  k_d = (vec3(1.0) - k_s) * (1.0 - metallic);	// 漫反射系数 -- 考虑金属度
	//vec3  k_d = (vec3(1.0) - k_s);					// 漫反射系数 -- 考虑金属度

	return k_d * irradiance * albedo;

}

vec3 pbr_ibl_specular_epic(vec3 n, vec3 v, vec3 albedo, vec3 f0, float roughness, float metallic) {
	vec3 res = vec3(0.0, 0.0, 0.0);

	float n_o_v = max(dot(n, v), 0.0);
	n_o_v = min(n_o_v, 0.999);												// bug 修正 -- 圆心白点

	vec3 f90 = 	brdf_f_f0(f0, albedo, metallic);							// 获取真正的 f0 -- 有金属性影响
	vec3 f = 	brdf_f_fresnel_schlick_roughness(n_o_v, f90, roughness);	// 菲涅尔方程

	vec3 R = reflect(-v, n);
	vec3 ld = textureLod(u_ibl_ld_map, R, roughness * u_max_reflection_lod).rgb;
	//vec3 ld = texture(u_ibl_ld_map, R, 0).rgb;
	vec2 dgf = texture(u_ibl_dgf_map, vec2(n_o_v, roughness)).rg;

	res = ld *(f * dgf.x + dgf.y);

	return res;
}

// ================================================================================
// light cac

float light_direct_att(LightDirect light_d) { return 1.0; }
vec3 light_direct_one(LightDirect light_d) {
	vec3 t_light_dir = normalize(-light_d.direction);
	vec3 t_f0 = vec3(0.04);			// 基础反射率 -- temp
	vec3 t_radiance = light_d.color * light_d.intensity * light_direct_att(light_d);

	vec3 res = pbr_Lo(	t_normal, t_view_dir, t_light_dir,
						t_c_diffuse, t_c_specular,
						t_f0, t_roughnes, t_metallic,
						t_radiance);
	return res;
}
vec3 light_direct() {
	vec3 res = vec3(0.0, 0.0, 0.0);
	for(int i = 0; i < u_light_direct_num; ++i) { res += light_direct_one(u_light_directs[i]); }
	return res;
}

vec3 light_ambient(vec3 coe) { 
	vec3 t_f0 = vec3(0.04);			// 基础反射率 -- temp
	vec3 diffuse = pbr_ibl_diffuse(	
						t_normal, t_view_dir,
						t_c_diffuse,
						t_f0, t_roughnes, t_metallic,
						t_irradiance_diffuse);

	vec3 specular = pbr_ibl_specular_epic(
						t_normal, t_view_dir,
						t_c_specular,
						t_f0, t_roughnes, t_metallic);

	return (diffuse + specular) * coe * t_ao; 
}

// ================================================================================











