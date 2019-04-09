#version 330 core

// light num 
const int max_direct_light_num	= 1;
const int max_point_light_num	= 4;
const int max_spot_light_num	= 4;
const int max_sky_light_num	    = 1;

out vec4 r_color;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	vec4 direct_light_pv_pos[max_direct_light_num];
} i_fs;

// base material
struct material {
    vec3 ka;
    vec3 kd;
    vec3 ks;

    sampler2D map_ka;       // 0                
    sampler2D map_kd;       // 1
    sampler2D map_ks;       // 2

	bool has_map_ka;
	bool has_map_kd;
	bool has_map_ks;

    float shininess;
};
// material for cac
struct material_helper {
	vec3 ka;
	vec3 kd;
	vec3 ks;

	vec4 map_ka_color;
	vec4 map_kd_color;
	vec4 map_ks_color;

	float shininess;
};

// light struct 
struct DirectLight {
    vec3 color;
    vec3 dirction;
    vec3 k;					// 各分量占比 -- ambient / diffuse / specular

    float intensity;

};
struct PointLight {
    vec3 color;
    vec3 position;
    vec3 k;

    float intensity;

    // att
    float att_ka, att_kb, att_kc;

};
struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 dirction;
    vec3 k;

    float intensity;
    
    // att
    float att_ka, att_kb, att_kc;

    // inner and outer cutoff
    float inner, outer;
    
};
struct SkyLight {
    vec3 color;
    vec3 k;

    float intensity;
    
};

// uniform variable

uniform DirectLight u_direct_light[max_direct_light_num];
uniform PointLight  u_point_light[max_point_light_num];
uniform SpotLight   u_spot_light[max_spot_light_num];
uniform SkyLight    u_sky_light[max_sky_light_num];
uniform int         u_direct_light_num;                         // default is 0
uniform int         u_point_light_num;
uniform int         u_spot_light_num;
uniform int         u_sky_light_num;

uniform sampler2D   u_direct_shadow_map_0;	// 3 阴影贴图 
uniform samplerCube u_point_shadow_map_0;	// 4 点光源阴影贴图 -- temp -- 以后改成 texture_array
uniform samplerCube u_point_shadow_map_1;	// 5
uniform samplerCube u_point_shadow_map_2;	// 6 
uniform samplerCube u_point_shadow_map_3;	// 7 

uniform vec3	    u_view_pos;
uniform material    u_material;

uniform float	u_far;

// helper variable
vec3 t_view_dir = vec3(0.0);
vec3 t_normal = vec3(0.0);
material_helper t_material_helper;
float direct_shadows[max_direct_light_num];
float point_shadows[max_point_light_num];

// cac shadow -- 预计算出所有光源产生的阴影
void cac_direct_shadow() {
    vec4 pos = i_fs.direct_light_pv_pos[0];
    vec3 proj_pos = pos.xyz / pos.w;							// 执行透视除法 -- 不是通过 gl_Position 传过来的 -- 所以要自己做
                                                                // 此时 proj_pos = [-1 ,, 1]
    proj_pos = proj_pos * 0.5 + 0.5;							// 变换到[0 ,, 1]的范围 -- 以获得采样坐标和 正确的深度[0,,1]
    //float closest_depth = texture(u_direct_shadow_map_0, proj_pos.xy).r;	
                                                                // 取得最近点的深度(使用[0,1]范围下的proj_pos当坐标)
    float cur_depth = proj_pos.z;								// 获得当前深度
    if(cur_depth > 1.0) cur_depth = 0.0;                        // 深度图以外的无阴影

    //float bias = 0.005;                                         // 阴影偏移 -- 解决阴影失真问题
    float bias = max(0.05 * (1.0 - dot(t_normal, t_view_dir)), 0.005);
    //direct_shadows[0] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	// 检查当前片元是否在阴影中 -- 在阴影中 -- 返回 1.0, 否则返回 0.0

    // use pcf
    direct_shadows[0] = 0.0;
    vec2 tex_size = 1.0 / textureSize(u_direct_shadow_map_0, 0);
    for(int x = -1; x <= 1; ++x) { for(int y = -1; y <= 1; ++y) {
        float pcf_depth = texture(u_direct_shadow_map_0, proj_pos.xy + vec2(x, y) * tex_size).r;	
        direct_shadows[0] += cur_depth > pcf_depth + bias ? 1.0 : 0.0;        
    }}
    direct_shadows[0] /= 9.0;

}
void cac_point_shadow_one() {
    vec3 pos_to_light =  i_fs.world_pos - u_point_light[0].position; 

    //float closest_depth = texture(u_point_shadow_map_0, pos_to_light).r;
    //closest_depth = clamp(closest_depth, 0.0, 1.0);
    //closest_depth *= u_far;

    float cur_depth = length(pos_to_light);

    float bias = 0.05; 
    //point_shadows[0] = cur_depth > closest_depth + bias  ? 1.0 : 0.0;	

    // use pcf
    point_shadows[0] = 0.0;
    float samples = 4.0;
    float offset = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closest_depth = texture(u_point_shadow_map_0, pos_to_light + vec3(x, y, z)).r; 
                closest_depth = clamp(closest_depth, 0.0, 1.0);
                closest_depth *= u_far;   // Undo mapping [0;1]
                if(cur_depth > closest_depth + bias) point_shadows[0] += 1.0;
            }
        }
    }
    point_shadows[0] /= (samples * samples * samples);
}

void cac_point_shadow_two() {

}
void cac_point_shadow_three() {

}
void cac_point_shadow_four() {

}
void cac_shadows() {
    for(int i = 0; i < u_direct_light_num; ++i) direct_shadows[i] = 0.0;
    for(int i = 0; i < u_point_light_num; ++i) point_shadows[i] = 0.0;

    if(u_direct_light_num > 0) cac_direct_shadow();
    if(u_point_light_num > 0) cac_point_shadow_one();
    if(u_point_light_num > 1) cac_point_shadow_two();
    if(u_point_light_num > 2) cac_point_shadow_three();
    if(u_point_light_num > 3) cac_point_shadow_four();
}

void pre_cac(vec3 view_pos) {
    t_view_dir = normalize(view_pos - i_fs.world_pos);
    t_normal = normalize(i_fs.normal);

    // init material_helper by uniform material
    t_material_helper.ka = u_material.ka;
    t_material_helper.kd = u_material.kd;
    t_material_helper.ks = u_material.ks;
    t_material_helper.shininess = clamp(u_material.shininess, 32.0, 128.0);
    if(u_material.has_map_ka) t_material_helper.map_ka_color = texture(u_material.map_ka, i_fs.tex_coord);
    else t_material_helper.map_ka_color = vec4(1.0);
    if(u_material.has_map_kd) t_material_helper.map_kd_color = texture(u_material.map_kd, i_fs.tex_coord);
    else t_material_helper.map_kd_color = vec4(1.0);
    if(u_material.has_map_ks) t_material_helper.map_ks_color = texture(u_material.map_ks, i_fs.tex_coord);
    else t_material_helper.map_ks_color = vec4(0.5);
	
	cac_shadows();
}
 
 // blinn_phong lighting
float cac_ambient() { return 1.0; }
float cac_diffuse(vec3 normal, vec3 light_dir) { return max(0.0, dot(light_dir, normal)); }
float cac_spcular(vec3 normal, vec3 light_dir, vec3 view_dir, float shininess) {
    vec3 t_halfway = normalize(light_dir + view_dir);
    return pow(max(0.0, dot(normal, t_halfway)), shininess);
}
vec3 blinn_phong(vec3 normal, vec3 light_dir, vec3 view_dir, material_helper mh, vec3 k, float shadow) {
    vec3 i_ambient = mh.ka * mh.map_ka_color.rgb * cac_ambient();    
    vec3 i_diffuse = mh.kd * mh.map_kd_color.rgb * cac_diffuse(normal, light_dir);
    vec3 i_spcular = mh.ks * mh.map_ks_color.rgb * cac_spcular(normal, light_dir, view_dir, mh.shininess);

    //return (1.0 - shadow) * (k.y * i_diffuse + k.z * i_spcular);
    return k.x * i_ambient + (1.0 - shadow) * (k.y * i_diffuse + k.z * i_spcular);
   
}

// cac light_attenuation
float att_dirct_light(DirectLight light) { return 1.0; }
float att_point_light(PointLight light) { 
    float dis = length(light.position - i_fs.world_pos);
    float att = 1.0 / (light.att_ka + light.att_kb*dis + light.att_kc*dis*dis);
    return att; 
}
float att_spott_light(SpotLight light) { 
    float dis = length(light.position - i_fs.world_pos);
    float att = 1.0 / (light.att_ka + light.att_kb*dis + light.att_kc*dis*dis);

    vec3 t_light_dir = normalize(light.position - i_fs.world_pos);
    float theta = dot(t_light_dir, normalize(-light.dirction));   // cos value
    float epsilon = light.inner - light.outer;          // cos value -- inner > outer
    float intensity = clamp((theta - light.outer) / epsilon, 0.0, 1.0);

    return att * intensity;
}

// cac light
vec3 cac_direct_light_one(int i) {
	DirectLight light = u_direct_light[i];

    vec3 t_c = light.color * light.intensity * att_dirct_light(light);

    vec3 res = t_c * blinn_phong(t_normal, -light.dirction, t_view_dir, t_material_helper, light.k, direct_shadows[i]);
    return res;
}
vec3 cac_point_light_one(int i) {
    PointLight light = u_point_light[i];

    vec3 t_light_dir = normalize(light.position - i_fs.world_pos);
    vec3 t_c = light.color * light.intensity * att_point_light(light);

    vec3 res = t_c * blinn_phong(t_normal, t_light_dir, t_view_dir, t_material_helper, light.k, point_shadows[i]);
    return res;
}
vec3 cac_spot_light_one(int i) {
    SpotLight light = u_spot_light[i];

    vec3 t_light_dir = normalize(light.position - i_fs.world_pos);
    vec3 t_c = light.color * light.intensity * att_spott_light(light);

    vec3 res = t_c * blinn_phong(t_normal, t_light_dir, t_view_dir, t_material_helper, light.k, 0.0);
    return res;
}
vec3 cac_sky_light_one(int i) {
    SkyLight light = u_sky_light[i];
    return vec3(0.0, 0.0, 0.0);
}
vec3 cac_direct_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);

    vec3 i_ambient = 0.1 * t_material_helper.ka * t_material_helper.map_ka_color.rgb * cac_ambient();    
    res += i_ambient;

    for(int i = 0; i < u_direct_light_num; ++i) { res += cac_direct_light_one(i); }
    return res;
}
vec3 cac_point_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_point_light_num; ++i) { res += cac_point_light_one(i); }
    return res;
}
vec3 cac_spot_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_spot_light_num; ++i) { res += cac_spot_light_one(i); }
    return res;
}
vec3 cac_sky_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_sky_light_num; ++i) { res += cac_sky_light_one(i); }
    return res;
}


void main(void) {

    pre_cac(u_view_pos);

    //vec3 t_color = u_light_color * blinn_phong(t_normal, t_light_dir, t_view_dir, t_material_helper);
    // t_color = vec3(1.0, 0.0, 0.0);
    //r_color = vec4(t_color, 1.0);

	//vec4 t_c = texture(u_material.map_kd, i_fs.tex_coord);
	//if(t_c.a < 0.01) discard;
    //r_color = t_c;

    vec3 t_color = vec3(0.0, 0.0, 0.0);
    t_color += cac_direct_light();
    t_color += cac_point_light();
    t_color += cac_spot_light();
    t_color += cac_sky_light();

    r_color = vec4(t_color, 1.0);
    // r_color = vec4(vec3(1.0 - point_shadows[0]), 1.0);
}

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/