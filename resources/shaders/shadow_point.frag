#version 330 core

in O_GS {
	vec4 world_pos;
} i_fs;

uniform vec3	u_light_pos;
uniform float	u_near;
uniform float	u_far;

void main() {
    float t_dis = length(i_fs.world_pos.xyz - u_light_pos);		// get distance between fragment and light source

    t_dis = (t_dis - u_near) / (u_far - u_near);			// map to [0;1] range by dividing by far_plane

    gl_FragDepth = t_dis;									// write this as modified depth

}