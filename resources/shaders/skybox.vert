#version 330 core
layout (location = 0) in vec3 a_pos;
 
// skybox vert shader

out O_VS {
	vec3 tex_coord;
} o_vs;

layout (std140) uniform Matrices {
	mat4 u_view;
	mat4 u_projection;
};

uniform mat4 u_view_sky_box;

void main() {
	
    o_vs.tex_coord = a_pos;		// 顶点坐标同时也是纹理坐标

    vec4 t_p = u_projection * u_view_sky_box * vec4(a_pos, 1.0f);
	gl_Position = t_p.xyww;		// 让 z 分量永远等于1 -- 透视除法之后 -- 无穷远 

}
