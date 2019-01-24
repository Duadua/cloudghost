#pragma once

#include <QOpenGLShader>

class QString;
class QOpenGLShaderProgram;

class Shader {
public:
	Shader() {}
	~Shader();

	Shader& use(); 
	void compile(const QString& v_path, const QString& f_path, const QString& g_path = nullptr);

	void set_int(const QString& name, const GLuint& value);
	void set_mat4(const QString& name, const QMatrix4x4& value);

private:
	void compile_shader(QOpenGLShader& shader, const QString& shader_path);
	void link_shaders();

private:
	QOpenGLShaderProgram* shader_program;	// 管理 shader 数据
};

