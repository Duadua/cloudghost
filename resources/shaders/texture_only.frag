#version 330 core

out vec4 r_color;

in O_VS {
	vec3 world_pos;
	vec3 normal;
	vec2 tex_coord;
	mat3 tbn;
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

// uniform variable
uniform material    u_material;

// helper variable
material_helper t_material_helper;

void pre_cac() {

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
	
}

vec4 get_texture_only() {
	vec4 res = vec4(0.0, 0.0, 0.0, 1.0);
	res = vec4(u_material.kd, 1.0);
	if(u_material.has_map_kd) res = texture(u_material.map_kd, i_fs.tex_coord);
	else if(u_material.has_map_ka) res = texture(u_material.map_ka, i_fs.tex_coord);	
	return res;
}
 
void main(void) {
    r_color = get_texture_only();
}

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/