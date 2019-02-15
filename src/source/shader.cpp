#include "shader.h"
#include <QString>
#include <QOpenGLShaderProgram>

IMPLEMENT_CLASS(Shader)

Shader::~Shader() { 
	delete shader_program;
}

SPTR_Shader Shader::use() { 
	shader_program->bind(); 
	return shared_from_this();
}

void Shader::compile(const std::string& v_path, const std::string& f_path, const std::string& g_path) {
	shader_program = new QOpenGLShaderProgram();

	// compile vertex shader
	QOpenGLShader v_shader(QOpenGLShader::Vertex);
	compile_shader(v_shader, v_path);

	// compile fragment shader
	QOpenGLShader f_shader(QOpenGLShader::Fragment);
	compile_shader(f_shader, f_path);

	// compile geometry shader
	if (g_path.compare("") != 0) { 
		QOpenGLShader g_shader(QOpenGLShader::Geometry);
		compile_shader(g_shader, g_path);
	}

	link_shaders();
}
void Shader::compile_shader(QOpenGLShader& shader, const std::string& shader_path) {
	bool res = shader.compileSourceFile(QString::fromStdString(shader_path));
	if (!res) {
		qDebug() << "¡¾error¡¿¡¾shader¡¿compile_failed" << endl;
		qDebug() << shader.log() << endl;
	}
	shader_program->addShader(&shader);
}
void Shader::link_shaders() {
	bool res = shader_program->link();
	if (!res) {
		qDebug() << "¡¾error¡¿¡¾shader¡¿link_failed" << endl;
		qDebug() << shader_program->log() << endl;
	}
}

void Shader::set_int(const std::string& name, const GLuint& value) {
	GLuint location = shader_program->uniformLocation(QString::fromStdString(name));
	shader_program->setUniformValue(location, value);
}
void Shader::set_mat4(const std::string& name, const QMatrix4x4& value) {
	GLuint location = shader_program->uniformLocation(QString::fromStdString(name));
	shader_program->setUniformValue(location, value);
}
