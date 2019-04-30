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
uniform int			u_pp_type;

// ================================================================================
// 后处理

// 后处理 const
const float offset = 1.0 / 300.0;  
// 后处理核
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

// 后处理方法
vec3 invers(vec3 color);					// 反相
vec3 gray(vec3 color, vec3 k);				// 灰度
vec3 edge_det();							// 边缘检测
vec3 blur();								// 模糊
vec3 sharpen();								// 锐化
vec3 real_rb3d(vec3 color);					// 红蓝 for fun

vec3 cac_kernel(float kernel[9]);			
vec3 pp(vec3 color, int type);

// ================================================================================
void main() {

	vec4 t_c = texture(u_texture, i_fs.tex_coord);
	r_color = vec4(pp(t_c.rgb, u_pp_type), t_c.a);
}

// ================================================================================

vec3 invers(vec3 color) { return vec3(1.0 - color); }
vec3 gray(vec3 color, vec3 k) {
	float t_av = (k.r * color.r + k.g * color.g + k.b * color.b);
	return vec3(t_av);
}
vec3 edge_det() {
	float kernel[9] = float[](
		1, 1, 1,
        1,-8, 1,
        1, 1, 1
    );
	return cac_kernel(kernel);
}
vec3 blur() {
	float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
	return cac_kernel(kernel);
}
vec3 sharpen() {
	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
    );
	return cac_kernel(kernel);
}
vec3 real_rb3d(vec3 color) {
	if(gl_FragCoord.x < 400) { return vec3(color.r, 0.0, 0.0); }	
	else { return vec3(0.0, 0.0, color.b); }
}

// ================================================================================

vec3 cac_kernel(float kernel[9]) {
	vec3 t_c[9];
    for(int i = 0; i < 9; i++) { t_c[i] = vec3(texture(u_texture, i_fs.tex_coord.st + offsets[i]).rgb); }

    vec3 res = vec3(0.0);
    for(int i = 0; i < 9; i++) { res += t_c[i] * kernel[i]; }
	return res;
}

vec3 pp(vec3 color, int type) {
	switch(type) {
	case 1 : return gray(color, vec3(0.2, 0.7, 0.1).rgb);
	case 2 : return invers(color); 
	case 3 : return blur(); 
	case 4 : return sharpen(); 
	case 5 : return edge_det(); 
	case 6 : return real_rb3d(color);
	}
	return color;
}

// ================================================================================