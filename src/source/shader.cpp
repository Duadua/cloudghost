#include "shader.h"
#include <QString>
#include <QOpenGLShaderProgram>

Shader::~Shader() { 
	//delete shader_program;
}

void Shader::compile(const QString& v_path, const QString& f_path, const QString& g_path) {
	shader_program = new QOpenGLShaderProgram();

	// compile vertex shader
	QOpenGLShader v_shader(QOpenGLShader::Vertex);
	compile_shader(v_shader, v_path);

	// compile fragment shader
	QOpenGLShader f_shader(QOpenGLShader::Fragment);
	compile_shader(f_shader, f_path);

	// compile geometry shader
	if (g_path != nullptr) { 
		QOpenGLShader g_shader(QOpenGLShader::Geometry);
		compile_shader(g_shader, g_path);
	}

	link_shaders();
}

Shader& Shader::use() { 
	shader_program->bind(); 
	return *this;
}

void Shader::compile_shader(QOpenGLShader& shader, const QString& shader_path) {
	bool res = shader.compileSourceFile(shader_path);
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

