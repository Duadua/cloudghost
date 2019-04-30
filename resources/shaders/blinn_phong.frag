#version 330 core

// ================================================================================
// const

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
} i_fs;

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

// uniform material
uniform Material    u_material;
uniform bool        u_normal_map_b_use;     // 是否使用法线贴图

// uniform for cac
uniform vec3    u_view_pos;

// ================================================================================
// pre cac

vec3 t_view_dir = vec3(0.0);
vec3 t_normal = vec3(0.0, 0.0, 1.0);
vec3 t_c_ambient = vec3(1.0);
vec3 t_c_diffuse = vec3(1.0);
vec3 t_c_specular = vec3(1.0);
float t_shininess = 32.0;

vec3 normal_from_texture();                     // 法线贴图

void pre_main();

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

// ================================================================================

void main(void) {

    // pre cac
    pre_main();

    // cac light
    vec3 t_color = vec3(0.0, 0.0, 0.0);
    t_color += light_direct();
    t_color += light_point();
    t_color += light_spot();

    r_color = vec4(t_color, 1.0);

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

    // init obj's color by uniform material
    t_c_ambient = u_material.ka;
    t_c_diffuse = u_material.kd;
    t_c_specular = u_material.ks;
    t_shininess = clamp(u_material.shininess, 8.0, 128.0);

    if(u_material.has_map_ka) t_c_ambient  *= texture(u_material.map_ka, i_fs.tex_coord).rgb;
    if(u_material.has_map_kd) t_c_diffuse  *= texture(u_material.map_kd, i_fs.tex_coord).rgb;
    if(u_material.has_map_ks) t_c_specular *= texture(u_material.map_ks, i_fs.tex_coord).rgb;

    t_view_dir = normalize(u_view_pos - i_fs.world_pos);

    if(u_normal_map_b_use && u_material.has_map_normal) { t_normal = normal_from_texture(); }
    else { t_normal = normalize(i_fs.normal); }
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

    return t_a + (1.0 - shadow) * (t_d + t_s);

}

// ================================================================================
// light cac

float light_direct_att(LightDirect light_d) { return 1.0; }
vec3  light_direct_one(LightDirect light_d, float shadow) {
    vec3 t_light_dir = normalize(-light_d.direction);
    vec3 t_radiance = light_d.color * light_d.intensity * 0.01 * light_direct_att(light_d);

    vec3 res = phong(   t_normal, t_view_dir, t_light_dir,
                        t_c_ambient, t_c_diffuse, t_c_specular,
                        t_shininess, shadow);

    return t_radiance * res;
}
vec3 light_direct() {
    vec3 res = vec3(0.0);
    for(int i = 0; i < u_light_direct_num; ++i) {
        res += light_direct_one(u_light_directs[i], 0.0);
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
        res += light_point_one(u_light_points[i], 0.0);
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

// ================================================================================

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/

// ================================================================================