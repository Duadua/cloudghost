#version 330 core

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

uniform sampler2D	u_texture;
uniform sampler2D	u_border_texture;
uniform vec4		u_border_color;

// 后处理核
const float offset = 1.0 / 300.0;  
vec2 offsets[9] = vec2[](
	vec2(-offset,  offset), // 左上
	vec2( 0.0f,    offset), // 正上
	vec2( offset,  offset), // 右上
	vec2(-offset,  0.0f),   // 左
	vec2( 0.0f,    0.0f),   // 中
	vec2( offset,  0.0f),   // 右
	vec2(-offset, -offset), // 左下
	vec2( 0.0f,   -offset), // 正下
	vec2( offset, -offset)  // 右下
);
vec3 cac_kernel(float kernel[9]) {
	vec3 t_c[9];
    for(int i = 0; i < 9; i++) { t_c[i] = vec3(texture(u_border_texture, i_fs.tex_coord.st + offsets[i]).rgb); }

    vec3 res = vec3(0.0);
    for(int i = 0; i < 9; i++) { res += t_c[i] * kernel[i]; }
	return res;
}
// 边缘检测
vec3 edge_det() {
	float kernel[9] = float[](
		1, 1, 1,
        1,-8, 1,
        1, 1, 1
    );
	return cac_kernel(kernel);
}

void main() {

	vec3 t_b = edge_det();
	vec4 t_c = texture(u_texture, i_fs.tex_coord);

	if(t_b.r > 0.9) { t_c.r = u_border_color.r; }			// 轮廓着色
	if(t_b.g > 0.9) { t_c.g = u_border_color.g; }			// 轮廓着色
	if(t_b.b > 0.9) { t_c.b = u_border_color.b; }			// 轮廓着色


	r_color = t_c;

}