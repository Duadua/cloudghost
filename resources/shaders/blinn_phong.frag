#version 330 core

// light num 
const int max_direct_light_num	= 1;
const int max_point_light_num	= 16;
const int max_spot_light_num	= 16;
const int max_sky_light_num	    = 1;

out vec4 r_color;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
} i_fs;

// base material
struct material {
    vec3 ka;
    vec3 kd;
    vec3 ks;

    sampler2D map_ka;                       
    sampler2D map_kd;
    sampler2D map_ks;

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
    vec3 k;             // 各分量占比 -- ambient / diffuse / specular

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

uniform vec3	    u_view_pos;
uniform material    u_material;

// helper variable
vec3 t_view_dir;
vec3 t_normal;
material_helper t_material_helper;

void pre_cac() {
    t_view_dir = normalize(u_view_pos - i_fs.world_pos);
    t_normal = normalize(i_fs.normal);

    // init material_helper by uniform material
    t_material_helper.ka = u_material.ka;
    t_material_helper.kd = u_material.kd;
    t_material_helper.ks = u_material.ks;
    t_material_helper.shininess = u_material.shininess;
    if(u_material.has_map_ka) t_material_helper.map_ka_color = texture(u_material.map_ka, i_fs.tex_coord);
    else t_material_helper.map_ka_color = vec4(1.0);
    if(u_material.has_map_kd) t_material_helper.map_kd_color = texture(u_material.map_kd, i_fs.tex_coord);
    else t_material_helper.map_kd_color = vec4(1.0);
    if(u_material.has_map_ks) t_material_helper.map_ks_color = texture(u_material.map_ks, i_fs.tex_coord);
    else t_material_helper.map_ks_color = vec4(1.0);
		
}
 
 // blinn_phong lighting
float cac_ambient() { return 1.0; }
float cac_diffuse(vec3 normal, vec3 light_dir) { return max(0.0, dot(light_dir, normal)); }
float cac_spcular(vec3 normal, vec3 light_dir, vec3 view_dir, float shininess) {
    vec3 t_halfway = normalize(light_dir + view_dir);
    return pow(max(0.0, dot(normal, t_halfway)), shininess);
}
vec3 blinn_phong(vec3 normal, vec3 light_dir, vec3 view_dir, material_helper mh, vec3 k) {
    vec3 i_ambient = mh.ka * mh.map_ka_color.rgb * cac_ambient();    
    vec3 i_diffuse = mh.kd * mh.map_kd_color.rgb * cac_diffuse(normal, light_dir);
    vec3 i_spcular = mh.ks * mh.map_ks_color.rgb * cac_spcular(normal, light_dir, view_dir, mh.shininess);

    return k.x * i_ambient + k.y * i_diffuse + k.z * i_spcular;
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
vec3 cac_direct_light_one(DirectLight light) {
    vec3 t_c = light.color * light.intensity * att_dirct_light(light);

    vec3 res = t_c * blinn_phong(t_normal, -light.dirction, t_view_dir, t_material_helper, light.k);
    return res;
}
vec3 cac_point_light_one(PointLight light) {
    vec3 t_light_dir = normalize(light.position - i_fs.world_pos);
    vec3 t_c = light.color * light.intensity * att_point_light(light);

    vec3 res = t_c * blinn_phong(t_normal, t_light_dir, t_view_dir, t_material_helper, light.k);
    return res;
}
vec3 cac_spot_light_one(SpotLight light) {
    vec3 t_light_dir = normalize(light.position - i_fs.world_pos);
    vec3 t_c = light.color * light.intensity * att_spott_light(light);

    vec3 res = t_c * blinn_phong(t_normal, t_light_dir, t_view_dir, t_material_helper, light.k);
    return res;
}
vec3 cac_sky_light_one(SkyLight light) {
    return vec3(0.0, 0.0, 0.0);
}
vec3 cac_direct_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_direct_light_num; ++i) { res += cac_direct_light_one(u_direct_light[i]); }
    return res;
}
vec3 cac_point_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_point_light_num; ++i) { res += cac_point_light_one(u_point_light[i]); }
    return res;
}
vec3 cac_spot_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_spot_light_num; ++i) { res += cac_spot_light_one(u_spot_light[i]); }
    return res;
}
vec3 cac_sky_light() {
    vec3 res = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < u_sky_light_num; ++i) { res += cac_sky_light_one(u_sky_light[i]); }
    return res;
}


void main(void) {

    pre_cac();

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
	
}

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/