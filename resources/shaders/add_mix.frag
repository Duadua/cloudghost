#version 330 core

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform sampler2D u_texture_a;
uniform sampler2D u_texture_b;

// ================================================================================

vec3 add_mix();

// ================================================================================


// ================================================================================

void main() {
	r_color = vec4(add_mix(), 1.0);
}

// ================================================================================

vec3 add_mix() {
	vec3 t_color_a = texture(u_texture_a, i_fs.tex_coord).rgb;
    vec3 t_color_b = texture(u_texture_b, i_fs.tex_coord).rgb;
	vec3 t_res = t_color_a + t_color_b; 
	if (isnan(t_res.r)) {
			//t_res = t_color_a;
			t_res = vec3(1.0, 0.0, 0.0);
	}
	return t_res;

}

// ================================================================================