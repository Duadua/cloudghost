#version 330 core

out vec4 r_color;

in vec3 o_world_pos;
in vec3 o_normal;
in vec2 o_tex_coord;

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


uniform vec3        u_light_pos;
uniform vec3	    u_light_color;
uniform vec3	    u_view_pos;
uniform material    u_material;

vec3 t_light_dir;
vec3 t_view_dir;
vec3 t_normal;
material_helper t_material_helper;

void pre_cac() {
    t_light_dir = normalize(u_light_pos - o_world_pos);
    t_view_dir = normalize(u_view_pos - o_world_pos);
    t_normal = normalize(o_normal);

    t_material_helper.ka = u_material.ka;
    t_material_helper.kd = u_material.kd;
    t_material_helper.ks = u_material.ks;
    t_material_helper.shininess = u_material.shininess;
    if(u_material.has_map_ka) t_material_helper.map_ka_color = texture(u_material.map_ka, o_tex_coord);
    else t_material_helper.map_ka_color = vec4(1.0);
    if(u_material.has_map_kd) t_material_helper.map_kd_color = texture(u_material.map_kd, o_tex_coord);
    else t_material_helper.map_kd_color = vec4(1.0);
    if(u_material.has_map_ks) t_material_helper.map_ks_color = texture(u_material.map_ks, o_tex_coord);
    else t_material_helper.map_ks_color = vec4(1.0);
		
}
 
vec3 cac_ambient() { return vec3(1.0); }
vec3 cac_diffuse() { return max(0.0, dot(t_light_dir, t_normal)); }
vec3 cac_spcular(float shininess) {
    vec3 t_halfway = normalize(t_light_dir + t_view_dir);
    return pow(max(0.0, dot(t_normal, t_halfway)), shininess);
}

vec3 blinn_phong(vec3 normal, vec3 light_dir, vec3 view_dir, material_helper mh, float att) {
    vec3 i_ambient = mh.ka * mh.map_ka_color * cac_ambient();    
    vec3 i_diffuse = mh.kd * mh.map_kd_color * cac_diffuse();
    vec3 i_spcular = mh.ks * mh.map_ks_color * cac_spcular(mh.shininess);

    return att * (i_ambient + i_diffuse + i_spcular);
    //return att * (i_diffuse);
	
}

float att_drect_light() { return 1.0; }
float att_point_light() { return 1.0; }
float att_spott_light() { return 1.0; }

void main(void) {

    pre_cac();

    vec3 t_color = u_light_color * blinn_phong(t_normal, t_light_dir, t_view_dir, t_material_helper, 1.0);
    // t_color = vec3(1.0, 0.0, 0.0);
    r_color = vec4(t_color, 1.0);

	//vec4 t_c = texture(u_material.map_kd, o_tex_coord);
	//if(t_c.a < 0.01) discard;
    //r_color = t_c;
}

/*
    o_* -- in from vertex shader
    u_* -- uniform
    r_* -- result to output
    t_* -- temp variable
*/