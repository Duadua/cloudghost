#version 330 core

out vec4 r_color;

in O_VS {
	vec2 tex_coord;
} i_fs;

uniform sampler2D	u_texture;			// 这里获得的是浮点纹理 -- 在此之前应该都是浮点纹理	-- 从而保证没有丢掉细节	
uniform int			u_type;
uniform float		u_exposure;			// 曝光参数

// tone mapping algorithm

vec3 reinhard(vec3 color) {
    vec3 res = color / (color + vec3(1.0));
	return res;
}

vec3 exposure(vec3 color, float exp) {
	vec3 res;
	return res;	
}

vec3 hdr(vec3 color, int type, float exp) {
	switch(type) {
	case 1 : return reinhard(color);
	case 2 : return exposure(color, exp);
	}
	return color;
}

void main() {
	vec4 t_c = texture(u_texture, i_fs.tex_coord);
	r_color = vec4(hdr(t_c.rgb, u_type, u_exposure), t_c.a);

}


