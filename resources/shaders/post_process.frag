#version 330 core

out vec4 r_color;

in vec2 o_tex_coord;

uniform sampler2D u_texture;



// 反相
vec3 invers(vec3 color) { return vec3(1.0 - color); }
// 灰度
vec3 gray(vec3 color, vec3 k) {
	float t_av = (k.r * color.r + k.g * color.g + k.b * color.b) / 3.0;
	return vec3(t_av);
}

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
    for(int i = 0; i < 9; i++) { t_c[i] = vec3(texture(u_texture, o_tex_coord.st + offsets[i]).rgb); }

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
// 模糊
vec3 blur() {
	float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
	return cac_kernel(kernel);
}
// 锐化
vec3 sharpen() {
	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
    );
	return cac_kernel(kernel);
}

vec3 pp(vec3 color) {
	//return sharpen();
	//return blur();
	return edge_det();
	return gray(color, vec3(0.3, 0.6, 0.1).rgb);
	return invers(color);
}

void main() {

	vec3 t_c = texture(u_texture, o_tex_coord).rgb;
	r_color = vec4(pp(t_c), 1.0);
}