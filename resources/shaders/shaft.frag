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
uniform vec3		u_center_pos;
uniform vec3		u_camera_pos;
uniform float		u_near;
uniform float		u_far;

uniform float		u_distance;

uniform mat4		u_inv_view;

// ================================================================================


// ================================================================================

float cac_depth(float p_depth);

float clampf(float x, float min_v, float max_v) { return min(max(x, min_v), max_v); }

// ================================================================================

void main() {
	// mask
	float t_cur_depth = texture(u_texture_depth, i_fs.tex_coord).r;
	vec3 t_pos_from_cam = i_fs.pos_from_cam * t_cur_depth;
	vec4 t_vec_from_world = u_inv_view * vec4(t_pos_from_cam, 1.0);
	vec3 t_pos_from_world = t_vec_from_world.xyz / t_vec_from_world.w;
	vec3 t_cur_from_light = t_pos_from_world - u_center_pos; 

	float dis = length(t_cur_from_light);
	float t_mask = 1.0 - clampf(dis/u_distance, 0.0, 1.0);
	//float t_t = u_distance - length(t_pos_from_cam);
    //float t_mask_min = 1.0 - clamp(t_t / u_distance, 0.0, 1.0);
	//t_mask = max(t_mask, t_mask_min);

	vec3 t_color = texture(u_texture, i_fs.tex_coord).rgb * t_mask;
	r_color = vec4(t_color, 1.0);
	//r_color = vec4(vec3(t_pos_from_world), 1.0);
}

// ================================================================================

// ================================================================================