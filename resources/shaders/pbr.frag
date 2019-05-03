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

// uniform for cac
uniform vec3 		u_view_pos;

// ================================================================================
// pbr 

// d g f
float brdf_d_tr_ggx(float n_o_h, float roughness);

float brdf_g_k_direct(float roughness);
float brdf_g_k_ibl(float roughness);
float brdf_g_schlick_ggx(float n_o_v, float k);
float brdf_g_smith(float n_o_v, float n_o_l, float k);

vec3  brdf_f_f0(vec3 f0, vec3 albedo, float metallic);		// albedo -- 反射率 -- 可能来自贴图 -- f0 为基础反射率
vec3  brdf_f_fresnel_schlick(float h_o_v, vec3 f0);			// h 为中间向量 -- 可作为微平面的法线 -- 有效的微平面

// brdf
vec3 brdf_cook_torrance(vec3 n, vec3 v, vec3 l, 
						vec3 c_diffuse, vec3 c_specular, 
						vec3 f0, float roughness, float metallic);

// 反射率方程
vec3 pbr_Lo(vec3 n, vec3 v, vec3 l,
			vec3 c_diffuse, vec3 c_specular, 
			vec3 f0, float roughness, float metallic,
			vec3 radiance);									// radiance -- 辐射度(入射)

// ================================================================================
// light cac

float light_direct_att(LightDirect light_d);
vec3  light_direct_one(LightDirect light_d);
vec3  light_direct();

vec3  light_ambient(vec3 coe); // coe -- 系数

// ================================================================================
// pre cac

vec3  t_c_ambient;
vec3  t_c_diffuse;
vec3  t_c_specular;

float t_metallic;
float t_roughnes;
float t_ao;

vec3  t_view_dir;
vec3  t_normal;

vec3 normal_from_texture();                     // 法线贴图

void pre_main(); 

// ================================================================================

void main() {
	pre_main();

	vec3 t_color = vec3(0.0, 0.0, 0.0);
	
	t_color += light_direct();

	t_color += light_ambient(vec3(0.1));

	r_color = vec4(t_color, 1.0);

	//r_color = vec4(vec3(t_ao), 1.0);
}

// ================================================================================
// pre cac

vec3 normal_from_texture() {
    // 缩小镜面光范围
    //t_shininess *= 64;

    // 切线空间的法线 --- 从法线贴图中得 [-1.0 ,, 1.0]
    vec3 tangent_normal = texture(u_material.map_normal, i_fs.tex_coord).rgb;
    tangent_normal = normalize(tangent_normal * 2.0 - 1.0);

    return normalize(i_fs.tbn * tangent_normal);
}

void pre_main() { 

	t_c_ambient = u_material.ka;
	t_c_diffuse =  u_material.kd;			// 片段本身基础颜色
	t_c_specular = u_material.ks;
	vec3 t_albedo = u_material.albedo;

	t_metallic = u_material.metallic;
	t_roughnes = u_material.roughness;
	t_ao = u_material.ao;

	if(u_material.has_map_ka) { t_c_ambient = texture(u_material.map_ka, i_fs.tex_coord).rgb; }
	if(u_material.has_map_kd) { t_c_diffuse = texture(u_material.map_kd, i_fs.tex_coord).rgb; }
	if(u_material.has_map_ks) { t_c_specular = texture(u_material.map_ks, i_fs.tex_coord).rgb; }
	if(u_material.has_map_albedo) { t_albedo = texture(u_material.map_albedo, i_fs.tex_coord).rgb; }

	if(u_material.has_map_metallic) { t_metallic = texture(u_material.map_metallic, i_fs.tex_coord).r; }
	if(u_material.has_map_roughness) { t_roughnes = texture(u_material.map_roughness, i_fs.tex_coord).r; }
	if(u_material.has_map_ao) { t_ao = texture(u_material.map_ao, i_fs.tex_coord).r; }

	// pbr 里修正颜色
	t_c_diffuse = t_albedo;
	t_c_specular = t_albedo;			 
	t_c_ambient = 1.0 * t_c_diffuse;
	t_c_ambient *= t_ao;

	t_view_dir = normalize(u_view_pos - i_fs.world_pos);

    if(u_normal_map_b_use && u_material.has_map_normal) { t_normal = normal_from_texture(); }
    else { t_normal = normalize(i_fs.normal); }
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

vec3  brdf_f_f0(vec3 f0, vec3 albedo, float metallic) {
	return mix(f0, albedo, metallic);
	// return metallic * albedo + (1.0 - metallic) * f0;
}
vec3 brdf_f_fresnel_schlick(float h_o_v, vec3 f0) {
	return f0 + (1.0 - f0) * pow(1.0 - h_o_v, 5.0);
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

vec3  light_ambient(vec3 coe) { return t_c_ambient * coe; }

// ================================================================================

/**
*	变量含义
*	l -- 光线向量 -- 从物体指向光源的向量 -- 小写的L (不是数字1)
*	v -- 视线向量 -- 从物体指向人眼的向量
*	h -- 半程向量 (halfway_vector) -- normalize(l + v) -- 用于一些计算 (Blinn_Phong 与 Phong 的区别)
*	n -- 法线向量 -- 物体平面的法线
*
*	k_s -- 镜面反射光百分比
*	k_d -- 漫反射光百分比 -- k_s + k_d = 100% -- 能量守恒
*/

/**
*	粗糙度 (Roughness)
*		- 一个平面越粗糙，则这个平面上的微平面的排列就越混乱 
*		- 从而入射光更会向不同的方向发散开 
*			- 产生分布范围更广的镜面反射 
 *			- 反之产生更锐利的镜面反射 
   *		- 但由于无法从像素来区分微平面的方向 -- 也就不好确认此平面的粗糙度
*		- 所以使用 粗糙度(Roughness) 作为参数，用统计学的方法概略估算平面的粗糙程度
*		- 粗糙度 -> h向量 与 此平面的微平面平均取向方向 一致的概率
*			- h向量 与微平面平均取向方向 越一致, 镜面反射的效果越锐利
*			- 粗糙度越小 -> 越一致
*/

/**
*	能量守恒
*	- 镜面光照(specular) -- 	一束光碰到一个表面时的反射部分
*	- 漫反射光照(diffuse) -- 	一束光碰到一个表面时的折射部分  
*		- 折射光线并不会立即被吸收, 它与碰到到的材料的微粒进行碰撞, 将光能转化为热能或动能(即光被微粒吸收)
*		- 并非所有的折射光线都会被吸收, 也有可能再次离开此表面 -- 次表面散射(Subsurface Scattering) -- 用来处理皮肤, 大理石, 蜡质等
*	- 反射(镜面反射光) + 折射(漫反射光) = 100%
*		- 假设反射光百分比计算结果为 k_s, 则折射光百分比为 k_d = (1.0 - k_s) 
*	- 金属(Metallic) 与 非金属 及 电介质(Dieletrics) 的表面在反射与折射时是不同的
*		- 金属中所有的折射光都会被直接吸走(不会再次散出) -- 不会有漫反射光 -- 只有镜面反射光
*		- 电介质 -- 一般为绝缘体, 可在电场作用下产生极化现象, 此时靠阴阳离子导电
*			- 玻璃 树脂 高分子聚合物 云母 瓷 橡胶 纸 聚苯乙烯
*			- 空气 氢气
*			- 石油 纯水
*			- 非金属
*/

/**
*	立体角(solid angle) -- ω
*	- 表示光束发散或会聚的程度(角度)
*	- 一个锥体(光锥) 投射到 单位球体表面上的表面积
*	- 以锥体的基点为球心作一个球表面, 锥体在球表面的截取部分的 表面积(dS) 与 球半径(r)平方 的比值	
*	- dΩ = dS / (r^2) = r^2 * sin(θ) * d(θ) * d(φ) / (r^2)
*		 			  = sin(θ)  * d(θ) * d(φ)
*		- 其中 θ 为pitch角(以垂直轴为原轴), φ 为yaw角, d() 的意思是微分
*	- 立体角的单位是 球面度 (sr)
*	- 球体的表面积为 4*(π)*r^2
*		- 所以一个光源向整个空间发出辐射能 或 一个物体从整个空间接收辐射能 的 立体角 为 4*(π) 球面度
*		- 半球空间所对应的立体角为 2*(π) 球面度 -- 半球领域(Hemisphere)
*		- 在 (θ) (φ) 角度范围内的立体角为
*			- W = ∫(θ)∫(φ) sin(θ) * d(θ) * d(φ)
*			- ∫() 的意思是积分
*	- 可以理解为一个带有体积的方向
*/

/**
*	辐射度量学(Radiometry)
*	- 辐射能 -- Q or W -- 单位 焦耳(J) -- 辐射的能量
*	- 辐射能密度 -- ω -- 单位 焦耳每立方米(J/(m^3)) -- 单位体积的辐射能 -- ω = d(Q) / d(v) -- 辐射能的空间特性
*	
*	- 辐射通量 -- Φ or P -- 单位 瓦特(W)(即焦耳每秒(J/s)) -- 单位时间的辐射能, 也叫辐射功率 -- P = d(Q) / d(t) -- t为时间段 -- 辐射能的时间特性
*	- 辐射强度 -- I -- 单位 瓦特每球面度(W/sr) -- 单位球面度的辐射通量 -- I = d(Φ) / d(Ω) -- 辐射能的方向特性
*
*	- 辐射亮度 -- L -- 单位  瓦特每球面度平方米(W/(sr*m^2))
*		- 光在其传输的 垂直方向上的 单位表面积单位立体角内发出的辐射通量
*		- L = d(I) / (d(A)*cos(θ))
*			= d(d(Φ) / d(Ω)) / (d(A)*cos(θ))
*			= d^2(Φ) / (d(Ω) * d(A)*cos(θ))
*			- θ 可理解为光源方向与(微)平面的法线之间的夹角
*			- A 为表面积
*		- 又称辐射度 or 辐射率 -- Radiance
*		- 如果把 立体角Ω和表面积A 都看作无穷小, 那么
*			- Ω 就可以转变为方向向量 ω -- 光源的方向
*			- A 就可以转变为某一点(片段) p
*			- L(p, ω) = d^2(Φ) / (d(ω) * d(p)*cos(θ))
*	
*	- 辐射照度 -- E -- 单位 瓦特每平方米(W/(m^2))
*		- 单位面元被照射的辐射通量 -- 强调接收 -- 相当于从各个方向(立体角为整个半球领域)的辐射通量
*		- E = d(Φ) / d(A)
*		- 由上面辐射率的公式可得
*			- L = d(d(Φ)/d(A)) / (d(ω) * cos(θ))
*				= d(E) / (d(ω) * cos(θ))
*		- 即
*			- d(E) = L * d(ω) * cos(θ)
*		- 那么, 像上面一样, 如果把 ω 看作方向向量(无穷小), 那么
*			- d(E(p, ω)) = L(p, ω) * d(ω) * cos(θ)
*/

/**
*	BRDF 双向反射分布函数
*	- 表示 某个观察方向上的出射的辐射通量 占 从某个光源入射方向上接收的总辐射能量 的 比例
*	- f(p, ω, v) = d(Lo(p, v)) / d(E(p, ω))
*		- p为照射的某一点(片断),  ω 为光源的入射方向, v 为观察方向
*	- BRDF 是由我们自己定义的模型, 它是计算的源头, 而非计算的结果
*		- 越接近现实情况的 BRDF 模型, 越能渲染出更好的效果
*		- 关于如何构建符合物理的  BRDF 模型, 或者如何理解已存在的比较合理的模型, 见下面
*/

/**
*	反射率方程 The Reflectance Equation
*	- 表示 某个观察方向上的出射的辐射通量 (入射原料为半球领域的光)	
*	- 由上面的 BRDF 公式可得
*		- d(Lo(p, v)) = f(p, ω, v) * d(E(p, ω))
*					  = f(p, ω, v) * L(p, ω) * d(ω) * cos(θ)
*		- 此 d(Lo(p, v)) 则是从观察方向 v 出射的辐射通量, 但入射原料为 ω 方向一个无穷小的球面度
*		- 所以对这个无穷小的球面度 ω 求一个 半球领域的积分, 就是我们最终要求的 某个观察方向的出射的辐射通量
*		- Lo(p, v)  = ∫_(Ω) (f(p, ωi, v) * L(p, ωi) * d(ωi) * cos(θi))
*	- 上面这个方程, 便是传说中的反射率方程
*	- θ 为 平面法线 与 入射光线 之间的夹角
*		- 所以 cos(θ) = dot(n, ω)
*		- Lo(p, v)  = ∫_(Ω) (f(p, ωi, v) * L(p, ωi) * d(ωi) * dot(n, ωi))
*	- 在实际 shader 里一般用离散化的方式求积分 -- 黎曼和 (Riemann sum)
*	- 此方程里
*		- f(p, ωi, v) 由我们自己确定模型之后参与计算
*		- L(p, ωi) 可以从光源处获得 -- 环境贴图 -- IBL 等有关
*/

/**
*	Cook-Torrance BRDF	方程使用
*	- f(p, ω, v) = k_d * f_lambert(p, ω, v) + k_s * f_cook_torrance(p, ω, v) 
*		- 其中 k_d 为漫反射系数(前面有提到), k_s 为镜面反射系数
*		- f_lambert(p, ω, v) = c_diffuse / π 
*			- c_diffuse 为物体的表面颜色 (surface color)
*			- 本来应该是 f_lambert(p, ω, v) = c*dot(n, ω) / π, 但是反射率方程里有了 dot(n, ω) 相当于提取到了外面
*				- 至于这个 π, 暂时没法理解
*		- f_cook_torrance(p, ω, v) = D * F * G / (4 * dot(v, n) * dot(ω, n))
*			- 后面详细解释这个方程
*/

/**
*	Cook-Torrance BRDF	方程推导
*	- 
*/

// ================================================================================














