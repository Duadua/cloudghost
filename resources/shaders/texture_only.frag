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
uniform bool		u_b_sphere_tex_coord;	// 是否使用球形的纹理坐标 -- 重新计算

// ================================================================================
// pre cac

vec2 t_tex_coord;

vec3 t_c_diffuse = vec3(1.0);

vec2 cac_sphere_tex_coord(vec3 v);              // 球形uv

void pre_main();

// ================================================================================

void main(void) {

    // pre cac
    pre_main();

    r_color = vec4(t_c_diffuse, 1.0);
}

// ================================================================================
// pre cac

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
    t_c_diffuse = u_material.kd;

    if(u_material.has_map_kd) t_c_diffuse  = texture(u_material.map_kd, t_tex_coord).rgb;
    else if(u_material.has_map_ka) t_c_diffuse = texture(u_material.map_ka, t_tex_coord).rgb;
}

// ================================================================================

/**
*	o_* -- in from vertex shader
*	u_* -- uniform
*	r_* -- result to output
*	t_* -- temp variable
*/

// ================================================================================