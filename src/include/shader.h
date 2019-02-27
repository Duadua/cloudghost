#pragma once

#include "cmath.h"
#include "reflex.h"
#include <string>
#include <QOpenGLShader>

class QOpenGLShaderProgram;

DECLARE_AUTO_PTR(Shader)
class Shader : public CObject, public std::enable_shared_from_this<Shader> {
	DECLARE_CLASS(Shader)
public:
	Shader() {}
	virtual ~Shader();

	SPTR_Shader use(); 
	void compile(const std::string& v_path, const std::string& f_path, const std::string& g_path = "");

	void set_int(const std::string& name, const uint& value);
	void set_float(const std::string& name, const float& value);
	void set_vec3(const std::string& name, const CVector3D& value);
	void set_mat4(const std::string& name, const CMatrix4x4& value);

private:
	void compile_shader(QOpenGLShader& shader, const std::string& shader_path);
	void link_shaders();

private:
	QOpenGLShaderProgram* shader_program;	// 管理 shader 数据
};


