#version 330 core

// ================================================================================
// const

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

// uniform material
uniform Material    u_material;

// ================================================================================
// pre cac

vec3 t_c_diffuse = vec3(1.0);

void pre_main();

// ================================================================================

void main(void) {

    // pre cac
    pre_main();

    r_color = vec4(t_c_diffuse, 1.0);
}

// ================================================================================
// pre cac

void pre_main() {

    // init obj's color by uniform material
    t_c_diffuse = u_material.kd;

    if(u_material.has_map_kd) t_c_diffuse  = texture(u_material.map_kd, i_fs.tex_coord).rgb;
    else if(u_material.has_map_ka) t_c_diffuse = texture(u_material.map_ka, i_fs.tex_coord).rgb;
}

// ================================================================================

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/

// ================================================================================