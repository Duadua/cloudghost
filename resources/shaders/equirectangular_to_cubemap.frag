#version 330 core

// ================================================================================
// const 

const float pi = acos(-1.0);

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec3 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform sampler2D	u_texture_hdr;

// ================================================================================

vec2 cac_sphere_tex_coord(vec3 tex);		// cubemap 的 tex 在 eq贴图上的 uv 坐标

// ================================================================================

void main() {
	vec2 tex = cac_sphere_tex_coord(normalize(i_fs.tex_coord));
	vec3 t_color = texture(u_texture_hdr, tex).rgb; 

	r_color = vec4(t_color, 1.0);
}

// ================================================================================

vec2 cac_sphere_tex_coord(vec3 v) {
	vec2 res = vec2(atan(v.z, v.x), asin(v.y)); // ([-pi ,, pi], [-pi/2.0 ,, pi/2.0])

	// 映射到 [0 ,, 1]
	//res.x = res.x / (2.0 * pi) + 0.5;
	//res.y = res.y / pi + 0.5;

	res = res / vec2(2.0 * pi, pi) + 0.5;

	return res;

}

// ================================================================================
