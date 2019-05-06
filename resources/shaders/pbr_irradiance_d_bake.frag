#version 330 core

// ================================================================================
// const 

const float pi = acos(-1.0);

// ================================================================================
// in & out

out vec4 r_color;

in O_VS {
	vec3 tex_coord;
} i_fs;

// ================================================================================
// uniform

uniform samplerCube	u_texture;

// ================================================================================
// pre cac

vec3 t_normal;
vec3 t_up;			// up 和 right -- 计算切线空间的 normal 用
vec3 t_right;

vec3 spherical_to_cartesian(float phi, float theta);	// 极坐标 -> 轴坐标 

void pre_main();


// ================================================================================
// 积分
vec3 integral(float delta);				// 按极坐标步进(pitch 和 yaw 都有)


// ================================================================================

void main() {

 // from the origin, aligned to WorldPos. Given this normal, calculate all
    // incoming radiance of the environment. The result of this radiance
    // is the radiance of light coming from -Normal direction, which is what
    // we use in the PBR shader to sample irradiance.
/*    vec3 N = normalize(i_fs.tex_coord);

    vec3 irradiance = vec3(0.0);   
    
    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, N);
    up            = cross(N, right);
       
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 

            irradiance += texture(u_texture, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    r_color = vec4(irradiance, 1.0);

*/

	// pre cac
	pre_main();

	vec3 t_color = integral(0.05);

	r_color = vec4(t_color, 1.0);


}

// ================================================================================
// pre cac

vec3 spherical_to_cartesian(float phi, float theta) {
	return vec3(
			sin(theta) * cos(phi), 
			sin(theta) * sin(phi),
			cos(theta)
		);
}

void pre_main() {

	// 切线空间向量基
	t_normal = normalize(i_fs.tex_coord);
	t_up = vec3(0.0, 1.0, 0.0);
	t_right = normalize(cross(t_up, t_normal));
	t_up = normalize(cross(t_normal, t_right));
}

// ================================================================================
// 积分

vec3 integral(float delta) {
	vec3 res = vec3(0.0);
	int cnt = 0;

	// 对半球领域积分 -- 黎曼和近似
	for(float phi = 0.0; phi < 2.0 * pi; phi += delta) {
		for(float theta = 0.0; theta < 0.5 * pi; theta += delta) {
			vec3 t_uv_tg = spherical_to_cartesian(phi, theta);							// 获得切线空间的方向向量
			vec3 t_uv = t_uv_tg.x * t_right + t_uv_tg.y * t_up + t_uv_tg.z * t_normal;	// 转化为世界空间的方向 

			vec4 t_Li = texture(u_texture, normalize(t_uv));							// 此方向上的辐射度

			res += t_Li.rgb * cos(theta) * sin(theta); 										// 积分量累加

			++cnt;																		// 统计积分个数
		}	
	}

	res = pi * (1.0 / float(cnt)) * res;
	return res;
}

