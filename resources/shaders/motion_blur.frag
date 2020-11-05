#version 330 core

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
    vec3 pos_from_cam;
} i_fs;

// ================================================================================
// uniform

uniform sampler2D	u_texture;
uniform sampler2D	u_texture_depth;
uniform float		u_sample_num;

uniform float		u_far;

uniform mat4		u_inv_view;
uniform mat4		u_last_proj_view;

// ================================================================================
// gauss blur

vec3 motion_blur(vec2 delta);

float clampf(float x, float min_v, float max_v) { return min(max(x, min_v), max_v); }

// ================================================================================
void main() {

	// cac cur ps's world pos
	float t_cur_depth = texture(u_texture_depth, i_fs.tex_coord).r;
	vec3 t_pos_from_cam = i_fs.pos_from_cam * t_cur_depth;
	vec4 t_vec_from_world = u_inv_view * vec4(t_pos_from_cam, 1.0);
	vec3 t_pos_from_world = t_vec_from_world.xyz / t_vec_from_world.w;

	// cac last frame's ps pos
	vec4 t_last_vec = u_last_proj_view * vec4(t_pos_from_world, 1.0); 
	vec3 t_last_pos = t_last_vec.xyz / t_last_vec.w;
	
	vec2 t_cur_pos = i_fs.tex_coord * vec2(2.0) - vec2(1.0); 
	vec2 t_delta = (t_cur_pos - t_last_pos.xy) / 5.0;
	if (t_cur_depth * u_far < 50) {
        r_color = vec4(texture(u_texture, i_fs.tex_coord).rgb, 1.0);
    }
	else {
        r_color = vec4(motion_blur(t_delta), 1.0);
    }
	//r_color = vec4(i_fs.pos_from_cam, 1.0);
}

// ================================================================================

vec3 motion_blur(vec2 delta)
{
    vec3 result = vec3(0.0);

	vec2 t_cur_uv = i_fs.tex_coord;
	for(int i = 0; i < u_sample_num; ++i) {
		vec3 t_cur_val = texture(u_texture, t_cur_uv).rgb;
		t_cur_uv = t_cur_uv + delta * 0.01;
		t_cur_uv.x = clampf(t_cur_uv.x, 0.0, 1.0);
		t_cur_uv.y = clampf(t_cur_uv.y, 0.0, 1.0);
		result += t_cur_val;
	}

    return result / (u_sample_num + 0.0);
}

// ================================================================================