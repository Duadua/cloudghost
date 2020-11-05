#version 330 core

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform sampler2D	u_texture;
uniform float		u_strength;
uniform float		u_sample_num;
uniform vec2		u_origin_scene_pos;

// ================================================================================
// gauss blur

vec3 radial_blur();

float clampf(float x, float min_v, float max_v) { return min(max(x, min_v), max_v); }

// ================================================================================
void main() {

	r_color = vec4(radial_blur(), 1.0);
}

// ================================================================================

vec3 radial_blur()
{
	vec2 tex_offset = 1.0 / textureSize(u_texture, 0);
    vec3 result = vec3(0.0);

	vec2 t_origin_uv = (u_origin_scene_pos + vec2(1.0)) / 2.0;  
    //t_origin_uv = vec2(0.5); 
	vec2 t_delta_uv = (t_origin_uv - i_fs.tex_coord) / (u_sample_num + 0.0) * u_strength;
	vec2 t_cur_uv = i_fs.tex_coord;

	for(int i = 0; i < u_sample_num; ++i) {
		vec3 t_cur_val = texture(u_texture, t_cur_uv).rgb;
		t_cur_uv = t_cur_uv + t_delta_uv;
		t_cur_uv.x = clampf(t_cur_uv.x, 0.0, 1.0);
		t_cur_uv.y = clampf(t_cur_uv.y, 0.0, 1.0);
		result += t_cur_val;
	}

    return result / (u_sample_num + 0.0);
}

// ================================================================================