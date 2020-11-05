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
uniform bool		ub_horizontal;

uniform float		u_weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

// ================================================================================
// gauss blur

vec3 gauss_blur();

// ================================================================================
void main() {

	r_color = vec4(gauss_blur(), 1.0);
}

// ================================================================================

vec3 gauss_blur()
{
	vec2 tex_offset = 1.0 / textureSize(u_texture, 0);
    vec3 result = texture(u_texture, i_fs.tex_coord).rgb * u_weight[0];
    if(ub_horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(u_texture, i_fs.tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * u_weight[i];
            result += texture(u_texture, i_fs.tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * u_weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(u_texture, i_fs.tex_coord + vec2(0.0, tex_offset.y * i)).rgb * u_weight[i];
            result += texture(u_texture, i_fs.tex_coord - vec2(0.0, tex_offset.y * i)).rgb * u_weight[i];
        }
    }
    return result;
}

// ================================================================================