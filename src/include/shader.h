#pragma once

#include "cmath.h"
#include "reflex.h"
#include <stack>
#include <string>

DECLARE_AUTO_PTR(Shader)
class Shader : public CObject, public std::enable_shared_from_this<Shader> {
	DECLARE_CLASS(Shader)
public:
	Shader();
	virtual ~Shader();

    GET_SET(std::string, name)

	SPTR_Shader use(); 
	void compile(const std::string& v_code, const std::string& f_code, const std::string& g_code = "");

	// set uniform
	void set_bool(const std::string& n, const bool& value);
	void set_int(const std::string& n, const int& value);
	void set_uint(const std::string& n, const uint& value);
	void set_float(const std::string& n, const float& value);

	void set_vec2(const std::string& n, const CVector2D& value);
	void set_vec3(const std::string& n, const CVector3D& value);
	void set_vec4(const std::string& n, const CVector4D& value);

	void set_vec2(const std::string& n, float x, float y);
	void set_vec3(const std::string& n, float x, float y, float z);
	void set_vec4(const std::string& n, float x, float y, float z, float w);

	void set_mat4(const std::string& n, const CMatrix4x4& value);

	void set_unifom_buffer(const std::string& n, uint idx);

private:
	uint compile_shader(const std::string& code, uint type);
	void link_shaders();

private:

	std::string name;

	uint shader_id;

	void check_compile_errors(uint id);
	void check_link_errors(uint id);
};

// =================================================================

DECLARE_AUTO_PTR(ShaderStack)
class ShaderStack : public CObject, public std::enable_shared_from_this<ShaderStack> {
	DECLARE_CLASS(ShaderStack)
public:
	ShaderStack();
	~ShaderStack() {}

	SPTR_ShaderStack push(const SPTR_Shader shader);
	SPTR_ShaderStack pop();

	SPTR_Shader top() const;

	SPTR_ShaderStack use();

	GET_SET(SPTR_Shader, shader_default)

private:
	std::stack<SPTR_Shader> shaders;
	SPTR_Shader shader_default;

};

// ================================================================================
// const 

const int light_direct_num_max = 1;
const int light_point_num_max = 4;
const int light_spot_num_max = 4;
const int light_sky_num_max = 1;

// 采样点 -- 贴图用
const int material_map_ka_id = 0;													// 0
const int material_map_kd_id = material_map_ka_id + 1;								// 1
const int material_map_ks_id = material_map_kd_id + 1;								// 2
const int material_map_normal_id = material_map_ks_id + 1;							// 3

const int material_map_albedo_id = material_map_normal_id + 1 + 1;					// 5
const int material_map_metallic_id = material_map_albedo_id + 1;					// 6
const int material_map_roughness_id = material_map_metallic_id + 1;					// 7
const int material_map_ao_id = material_map_roughness_id + 1;						// 8

const int shadow_direct_0_id = 12;													// 12 -- 阴影贴图用
const int shadow_point_0_id = shadow_direct_0_id + light_direct_num_max;			// 13
const int shadow_spot_0_id = shadow_point_0_id + light_point_num_max;				// 17



