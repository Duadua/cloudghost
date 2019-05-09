#version 330 core

// ================================================================================
// const

const float pi = acos(-1.0);

// light num 
const int light_direct_num_max  = 1;
const int light_point_num_max	= 4;
const int light_spot_num_max	= 4;
const int light_sky_num_max	    = 1;

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	mat3 tbn;
	vec4 light_direct_pv_pos[light_direct_num_max];
} i_fs;

in O_APos {
    vec3 world_pos;             // 没有任何变换的坐标
} i_apos;

// ================================================================================
// light

struct LightDirect {
    vec3 color;
    vec3 direction;
    vec3 k;					// 各分量占比 -- ambient / diffuse / specular

    float intensity;

};
struct LightPoint {
    vec3 color;
    vec3 position;
    vec3 k;

    float intensity;

    // att
    float att_ka, att_kb, att_kc;

};
struct LightSpot {
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 k;

    float intensity;
    
    // att
    float att_ka, att_kb, att_kc;

    // inner and outer cutoff
    float inner, outer;
    
};
struct LightSky {
    vec3 color;
    vec3 k;

    float intensity;
    
};

// ================================================================================
// material

// base material
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
};

// ================================================================================
// uniform

// uniform light
uniform LightDirect u_light_directs[light_direct_num_max];
uniform LightPoint  u_light_points[light_point_num_max];
uniform LightSpot   u_light_spots[light_spot_num_max];
uniform LightSky    u_light_skys[light_sky_num_max];
uniform int         u_light_direct_num;                         // default is 0
uniform int         u_light_point_num;
uniform int         u_light_spot_num;
uniform int         u_light_sky_num;

// uniform shadow
uniform sampler2D   u_shadow_direct_map_0;	// 5 阴影贴图 
uniform samplerCube u_shadow_point_map_0;	// 6 点光源阴影贴图 -- temp -- 以后改成 texture_array
uniform samplerCube u_shadow_point_map_1;	// 7
uniform samplerCube u_shadow_point_map_2;	// 8
uniform samplerCube u_shadow_point_map_3;	// 9
uniform bool        u_shadow_b_use;

// uniform material
uniform Material    u_material;
uniform bool        u_normal_map_b_use;     // 是否使用法线贴图
uniform bool		u_b_sphere_tex_coord;	// 是否使用球形的纹理坐标 -- 重新计算

// uniform for cac
uniform vec3        u_view_pos;
uniform float	    u_far;

// ================================================================================
// pre cac

vec2 t_tex_coord;

vec3 t_c_ambient = vec3(1.0);
vec3 t_c_diffuse = vec3(1.0);
vec3 t_c_specular = vec3(1.0);
float t_shininess = 32.0;

vec3 t_view_dir = vec3(0.0);
vec3 t_normal = vec3(0.0, 0.0, 1.0);

vec3 normal_from_texture();                     // 法线贴图

vec2 cac_sphere_tex_coord(vec3 v);              // 球形uv

void pre_main();

// ================================================================================
// shadow

float shadow_directs[light_direct_num_max];
float shadow_points[light_point_num_max];
float shadow_spots[light_spot_num_max];

const int shadow_point_sample_num = 20; 
vec3 shadow_point_sampler_offsets[shadow_point_sample_num] = vec3[] (
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

void shadow_direct();
void shadow_point();

void shadow_direct_one();
void shadow_point_one();
void shadow_point_two();
void shadow_point_three();
void shadow_point_four();

// ================================================================================
// blinn_phong

float phong_ambient();
float phong_diffuse(float n_o_l);
float phong_specular(float n_o_h, float shininess);

vec3  phong( vec3 n, vec3 v, vec3 l,                            // n v l  法线 视线 光线 方向向量
            vec3 c_ambient, vec3 c_diffuse, vec3 c_specular,    // 颜色
            float shininess, float shadow);                     // 反光度 阴影

// ================================================================================
// light cac

float light_direct_att(LightDirect light_d);
vec3  light_direct_one(LightDirect light_d, float shadow);
vec3  light_direct();

float light_point_att(LightPoint light_p);
vec3  light_point_one(LightPoint light_p, float shadow);
vec3  light_point();

float light_spot_att(LightSpot light_s);
vec3  light_spot_one(LightSpot light_s,float shadow);
vec3  light_spot();

vec3  light_ambient(vec3 coe);   // 环境光

// ================================================================================

void main(void) {

    // pre cac
    pre_main();

    // cac shadow
    shadow_direct();
    shadow_point();
    //shadow_spot();

    // cac light
    vec3 t_color = vec3(0.0, 0.0, 0.0);
    t_color += light_direct();
    t_color += light_point();
    t_color += light_spot();

    t_color += light_ambient(vec3(1.0));

    r_color = vec4(t_color, 1.0);
    //r_color = vec4(vec3(1.0 - point_shadows[0]), 1.0);
    //r_color = vec4(vec3(1.0 - direct_shadows[0]), 1.0);

    //r_color = vec4(t_normal * 0.5 + 0.5, 1.0);

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

    // init obj's color by uniform material
    t_c_ambient = u_material.ka;
    t_c_diffuse = u_material.kd;
    t_c_specular = u_material.ks;
    t_shininess = clamp(u_material.shininess, 8.0, 256.0);

    if(u_material.has_map_ka) t_c_ambient  *= texture(u_material.map_ka, t_tex_coord).rgb;
    if(u_material.has_map_kd) t_c_diffuse  *= texture(u_material.map_kd, t_tex_coord).rgb;
    if(u_material.has_map_ks) t_c_specular *= texture(u_material.map_ks, t_tex_coord).rgb;

    t_view_dir = normalize(u_view_pos - i_fs.world_pos);

    if(u_normal_map_b_use && u_material.has_map_normal) { t_normal = normal_from_texture(); }
    else { t_normal = normalize(i_fs.normal); }
}

// ================================================================================
// shadow
 
 // shadow direct light 
void shadow_direct() {
    for(int i = 0; i < u_light_direct_num; ++i) shadow_directs[i] = 0.0;
    if(!u_shadow_b_use) return;

    if(u_light_direct_num > 0) shadow_direct_one();

}
void shadow_direct_one() {
    vec4 pos = i_fs.light_direct_pv_pos[0];
    vec3 proj_pos = pos.xyz / pos.w;							// 执行透视除法 -- 不是通过 gl_Position 传过来的 -- 所以要自己做
                                                                // 此时 proj_pos = [-1 ,, 1]
    proj_pos = proj_pos * 0.5 + 0.5;							// 变换到[0 ,, 1]的范围 -- 以获得采样坐标和 正确的深度[0,,1]
    //float closest_depth = texture(u_shadow_direct_map_0, proj_pos.xy).r;	
                                                                // 取得最近点的深度(使用[0,1]范围下的proj_pos当坐标)
    float cur_depth = proj_pos.z;								// 获得当前深度
    if(cur_depth > 1.0) cur_depth = 0.0;                        // 深度图以外的无阴影

    float bias = 0.005;                                         // 阴影偏移 -- 解决阴影失真问题
    //float bias = max(0.05 * (1.0 - dot(t_normal, t_view_dir)), 0.005);
    //shadow_directs[0] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	// 检查当前片元是否在阴影中 -- 在阴影中 -- 返回 1.0, 否则返回 0.0

    // use pcf
    shadow_directs[0] = 0.0;
    vec2 tex_size = 1.0 / textureSize(u_shadow_direct_map_0, 0);
    for(int x = -1; x <= 1; ++x) { for(int y = -1; y <= 1; ++y) {
        float pcf_depth = texture(u_shadow_direct_map_0, proj_pos.xy + vec2(x, y) * tex_size).r;	
        shadow_directs[0] += cur_depth > pcf_depth + bias ? 1.0 : 0.0;        
    }}
    shadow_directs[0] /= 9.0;
}

// shadow point light
void shadow_point() {
    for(int i = 0; i < u_light_point_num; ++i) shadow_points[i] = 0.0;
    if(!u_shadow_b_use) return;

    if(u_light_point_num > 0) shadow_point_one();
    if(u_light_point_num > 1) shadow_point_two();
    if(u_light_point_num > 2) shadow_point_three();
    if(u_light_point_num > 3) shadow_point_four();

}
void shadow_point_one() {
    vec3 pos_to_light =  i_fs.world_pos - u_light_points[0].position; 

    //float closest_depth = texture(u_shadow_point_map_0, pos_to_light).r;
    //closest_depth = clamp(closest_depth, 0.0, 1.0);
    //closest_depth *= u_far;

    float cur_depth = length(pos_to_light);
    if(cur_depth > u_far) cur_depth = 0.0;                        // 深度图以外的无阴影

    float bias = 0.05; 
    //shadow_points[0] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	

    // use pcf
    shadow_points[0] = 0.0;
    float samples = 3.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closest_depth = texture(u_shadow_point_map_0, pos_to_light + vec3(x, y, z)).r; 
                closest_depth = clamp(closest_depth, 0.0, 1.0);
                closest_depth *= u_far;   // Undo mapping [0;1]
                if(cur_depth > closest_depth + bias) shadow_points[0] += 1.0;
            }
        }
    }
    shadow_points[0] /= (samples * samples * samples);

    /*shadow_points[0] = 0.0;
    float disk_radius = (1.0 + (length(u_view_pos - i_fs.world_pos) / u_far)) / 25.0;
    for(int i = 0; i < shadow_point_sample_num; ++i) {
        float closest_depth = texture(u_shadow_point_map_0, pos_to_light + shadow_point_sampler_offsets[i] * disk_radius).r;
        closest_depth *= u_far;   // Undo mapping [0;1]
        if(cur_depth > closest_depth + bias) shadow_points[0] += 1.0;
    }
    shadow_points[0] /= float(shadow_point_sample_num); */
    
}
void shadow_point_two() {
    vec3 pos_to_light =  i_fs.world_pos - u_light_points[1].position; 

    //float closest_depth = texture(u_shadow_point_map_1, pos_to_light).r;
    //closest_depth = clamp(closest_depth, 0.0, 1.0);
    //closest_depth *= u_far;

    float cur_depth = length(pos_to_light);
    if(cur_depth > u_far) cur_depth = 0.0;                        // 深度图以外的无阴影

    float bias = 0.05; 
    //shadow_points[0] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	

    // use pcf
    shadow_points[1] = 0.0;
    float samples = 3.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closest_depth = texture(u_shadow_point_map_1, pos_to_light + vec3(x, y, z)).r; 
                closest_depth = clamp(closest_depth, 0.0, 1.0);
                closest_depth *= u_far;   // Undo mapping [0;1]
                if(cur_depth > closest_depth + bias) shadow_points[1] += 1.0;
            }
        }
    }
    shadow_points[1] /= (samples * samples * samples);

    /*shadow_points[1] = 0.0;
    float disk_radius = (1.0 + (length(u_view_pos - i_fs.world_pos) / u_far)) / 25.0;
    for(int i = 0; i < shadow_point_sample_num; ++i) {
        float closest_depth = texture(u_shadow_point_map_1, pos_to_light + shadow_point_sampler_offsets[i] * disk_radius).r;
        closest_depth *= u_far;   // Undo mapping [0;1]
        if(cur_depth > closest_depth + bias) shadow_points[1] += 1.0;
    }
    shadow_points[1] /= float(shadow_point_sample_num); */
    
}
void shadow_point_three() {
 vec3 pos_to_light =  i_fs.world_pos - u_light_points[2].position; 

    //float closest_depth = texture(u_shadow_point_map_2, pos_to_light).r;
    //closest_depth = clamp(closest_depth, 0.0, 1.0);
    //closest_depth *= u_far;

    float cur_depth = length(pos_to_light);
    if(cur_depth > u_far) cur_depth = 0.0;                        // 深度图以外的无阴影

    float bias = 0.05; 
    //shadow_points[2] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	

    // use pcf
    shadow_points[2] = 0.0;
    float samples = 3.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closest_depth = texture(u_shadow_point_map_2, pos_to_light + vec3(x, y, z)).r; 
                closest_depth = clamp(closest_depth, 0.0, 1.0);
                closest_depth *= u_far;   // Undo mapping [0;1]
                if(cur_depth > closest_depth + bias) shadow_points[2] += 1.0;
            }
        }
    }
    shadow_points[2] /= (samples * samples * samples);

    /*shadow_points[2] = 0.0;
    float disk_radius = (1.0 + (length(u_view_pos - i_fs.world_pos) / u_far)) / 25.0;
    for(int i = 0; i < shadow_point_sample_num; ++i) {
        float closest_depth = texture(u_shadow_point_map_2, pos_to_light + shadow_point_sampler_offsets[i] * disk_radius).r;
        closest_depth *= u_far;   // Undo mapping [0;1]
        if(cur_depth > closest_depth + bias) shadow_points[2] += 1.0;
    }
    shadow_points[2] /= float(shadow_point_sample_num); */
    
}
void shadow_point_four() {
 vec3 pos_to_light =  i_fs.world_pos - u_light_points[3].position; 

    //float closest_depth = texture(u_shadow_point_map_3, pos_to_light).r;
    //closest_depth = clamp(closest_depth, 0.0, 1.0);
    //closest_depth *= u_far;

    float cur_depth = length(pos_to_light);
    if(cur_depth > u_far) cur_depth = 0.0;                        // 深度图以外的无阴影

    float bias = 0.05; 
    //shadow_points[3] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	

    // use pcf
    shadow_points[3] = 0.0;
    float samples = 3.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closest_depth = texture(u_shadow_point_map_3, pos_to_light + vec3(x, y, z)).r; 
                closest_depth = clamp(closest_depth, 0.0, 1.0);
                closest_depth *= u_far;   // Undo mapping [0;1]
                if(cur_depth > closest_depth + bias) shadow_points[3] += 1.0;
            }
        }
    }
    shadow_points[3] /= (samples * samples * samples);

    /*shadow_points[3] = 0.0;
    float disk_radius = (1.0 + (length(u_view_pos - i_fs.world_pos) / u_far)) / 25.0;
    for(int i = 0; i < shadow_point_sample_num; ++i) {
        float closest_depth = texture(u_shadow_point_map_3, pos_to_light + shadow_point_sampler_offsets[i] * disk_radius).r;
        closest_depth *= u_far;   // Undo mapping [0;1]
        if(cur_depth > closest_depth + bias) shadow_points[3] += 1.0;
    }
    shadow_points[3] /= float(shadow_point_sample_num); */
    
}

// ================================================================================
// blinn_phong

float phong_ambient() { return 1.0; }
float phong_diffuse(float n_o_l) { return n_o_l; }
float phong_specular(float n_o_h, float shininess) { return pow(n_o_h, shininess); }

vec3 phong( vec3 n, vec3 v, vec3 l, vec3 c_ambient, vec3 c_diffuse, vec3 c_specular, float shininess, float shadow) {
    vec3 h = normalize(v + l);
    float n_o_l = max(dot(n, l), 0.0);
    float n_o_h = max(dot(n, h), 0.0);

    vec3 t_a = phong_ambient() * c_ambient;
    vec3 t_d = phong_diffuse(n_o_l) * c_diffuse;
    vec3 t_s = phong_specular(n_o_h, shininess) * c_specular;

    return (1.0 - shadow) * (0.8 * t_d + 0.2 * t_s);
    //return t_a + (1.0 - shadow) * (t_d + t_s);

}

// ================================================================================
// light cac

float light_direct_att(LightDirect light_d) { return 1.0; }
vec3  light_direct_one(LightDirect light_d, float shadow) {
    vec3 t_light_dir = normalize(-light_d.direction);
    vec3 t_radiance = light_d.color * light_d.intensity * 0.1 * light_direct_att(light_d);

    vec3 res = phong(   t_normal, t_view_dir, t_light_dir,
                        t_c_ambient, t_c_diffuse, t_c_specular,
                        t_shininess, shadow);

    return t_radiance * res;
}
vec3 light_direct() {
    vec3 res = vec3(0.0);
    for(int i = 0; i < u_light_direct_num; ++i) {
        res += light_direct_one(u_light_directs[i], shadow_directs[i]);
    }
    return res;
}

float light_point_att(LightPoint light_p) {
    float dis = length(light_p.position - i_fs.world_pos);
    float att = 1.0 / (light_p.att_ka + light_p.att_kb*dis + light_p.att_kc*dis*dis);
    return att; 
}
vec3 light_point_one(LightPoint light_p, float shadow) {
    vec3 t_light_dir = normalize(light_p.position - i_fs.world_pos);
    vec3 t_radiance = light_p.color * light_p.intensity * light_point_att(light_p);

    vec3 res = phong(   t_normal, t_view_dir, t_light_dir,
                        t_c_ambient, t_c_diffuse, t_c_specular,
                        t_shininess, shadow);

    return t_radiance * res;
    
}
vec3 light_point() {
    vec3 res = vec3(0.0);
    for(int i = 0; i < u_light_point_num; ++i) {
        res += light_point_one(u_light_points[i], shadow_points[i]);
    }
    return res;

}

float light_spot_att(LightSpot light_s) {
    float dis = length(light_s.position - i_fs.world_pos);
    float att = 1.0 / (light_s.att_ka + light_s.att_kb*dis + light_s.att_kc*dis*dis);

    // 边缘平滑
    vec3 t_light_dir = normalize(light_s.position - i_fs.world_pos);
    float theta = dot(t_light_dir, normalize(-light_s.direction));          // cos value
    float epsilon = light_s.inner - light_s.outer;                          // cos value -- inner > outer
    float intensity = clamp((theta - light_s.outer) / epsilon, 0.0, 1.0);

    return att * intensity;

}
vec3 light_spot_one(LightSpot light_s,float shadow) {
    vec3 t_light_dir = normalize(light_s.position - i_fs.world_pos);
    vec3 t_radiance = light_s.color * light_s.intensity * light_spot_att(light_s);

    vec3 res = phong(   t_normal, t_view_dir, t_light_dir,
                        t_c_ambient, t_c_diffuse, t_c_specular,
                        t_shininess, shadow);

    return t_radiance * res;
}
vec3 light_spot() {
    vec3 res = vec3(0.0);
    for(int i = 0; i < u_light_spot_num; ++i) {
        res += light_spot_one(u_light_spots[i], 0.0);
    }
    return res;   
}

vec3 light_ambient(vec3 coe) { return t_c_ambient * coe; }

// ================================================================================

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/

// ================================================================================