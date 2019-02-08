#pragma once

#include <QOpenGLShader>
#include "reflex.h"

class QString;
class QOpenGLShaderProgram;

DECLARE_AUTO_PTR(Shader)
class Shader : public CObject, public std::enable_shared_from_this<Shader> {
	DECLARE_CLASS(Shader)
public:
	Shader() {}
	virtual ~Shader();

	SPTR_Shader use(); 
	void compile(const QString& v_path, const QString& f_path, const QString& g_path = nullptr);

	void set_int(const QString& name, const GLuint& value);
	void set_mat4(const QString& name, const QMatrix4x4& value);

private:
	void compile_shader(QOpenGLShader& shader, const QString& shader_path);
	void link_shaders();

private:
	QOpenGLShaderProgram* shader_program;	// 管理 shader 数据
};


