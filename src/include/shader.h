#pragma once

#include "cmath.h"
#include "reflex.h"
#include <string>
#include <QOpenGLFunctions_3_3_Core>

DECLARE_AUTO_PTR(Shader)
class Shader : public CObject, public std::enable_shared_from_this<Shader> {
	DECLARE_CLASS(Shader)
public:
	Shader() {}
	virtual ~Shader();

	GET_SET(std::string, name);

	SPTR_Shader use(); 
	void compile(const std::string& v_code, const std::string& f_code, const std::string& g_code = "");

	// set uniform
	void set_bool(const std::string& name, const bool& value);
	void set_int(const std::string& name, const int& value);
	void set_uint(const std::string& name, const uint& value);
	void set_float(const std::string& name, const float& value);

	void set_vec2(const std::string& name, const CVector2D& value);
	void set_vec3(const std::string& name, const CVector3D& value);
	void set_vec4(const std::string& name, const CVector4D& value);

	void set_vec2(const std::string& name, float x, float y);
	void set_vec3(const std::string& name, float x, float y, float z);
	void set_vec4(const std::string& name, float x, float y, float z, float w);

	void set_mat4(const std::string& name, const CMatrix4x4& value);

private:
	uint compile_shader(const std::string& code, uint type);
	void link_shaders();

private:

	std::string name;

	uint shader_id;

	QOpenGLFunctions_3_3_Core* core;

	void check_compile_errors(uint id);
	void check_link_errors(uint id);
};


