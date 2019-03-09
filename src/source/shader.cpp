#include "shader.h"
#include <QDebug>

IMPLEMENT_CLASS(Shader)

Shader::~Shader() { }

SPTR_Shader Shader::use() { 
	core->glUseProgram(shader_id);
	return shared_from_this();
}

void Shader::compile(const std::string& v_code, const std::string& f_code, const std::string& g_code) {
	uint v_id, f_id, g_id;
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	shader_id = core->glCreateProgram();

	// compile vertex shader
	v_id = compile_shader(v_code, GL_VERTEX_SHADER);

	// compile fragment shader
	f_id = compile_shader(f_code, GL_FRAGMENT_SHADER);

	// compile geometry shader
	if (g_code.compare("") != 0) { g_id = compile_shader(g_code, GL_GEOMETRY_SHADER); }

	link_shaders();

	core->glDeleteShader(v_id);
	core->glDeleteShader(f_id);
	if(g_code.compare("") != 0) core->glDeleteShader(g_id);
}

uint Shader::compile_shader(const std::string& code, uint type) {
	const char* t_code = code.c_str();
	uint id = core->glCreateShader(type);
	core->glShaderSource(id, 1, &t_code, NULL);

	core->glCompileShader(id);
	check_compile_errors(id);

	core->glAttachShader(shader_id, id);
	return id;
}

void Shader::link_shaders() {
	core->glLinkProgram(shader_id);
	check_link_errors(shader_id);
}

void Shader::check_compile_errors(uint id) {
	int flag;
	char res[1024];
	core->glGetShaderiv(id, GL_COMPILE_STATUS, &flag);
	if (!flag) {
		core->glGetShaderInfoLog(id, 1024, NULL, res);
		std::string info = "【error】【shader】【compile】\n";
		info.append(res);
		info.append("\n -- ----------------------------------------");
		qDebug() << QString::fromStdString(info);
	}
}
void Shader::check_link_errors(uint id) {
	int flag;
	char res[1024];
	core->glGetProgramiv(id, GL_LINK_STATUS, &flag);
	if (!flag) {
		core->glGetProgramInfoLog(id, 1024, NULL, res);
		std::string info = "【error】【shader】【link】\n";
		info.append(res);
		info.append("\n -- ----------------------------------------");
		qDebug() << QString::fromStdString(info);
	}
}

void Shader::set_bool(const std::string& name, const bool& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform1i(location, (int)value);
}
void Shader::set_int(const std::string& name, const int& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform1i(location, value);
}
void Shader::set_uint(const std::string& name, const uint& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform1ui(location, value);
}

void Shader::set_float(const std::string& name, const float& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform1f(location, value);
}

void Shader::set_vec2(const std::string& name, const CVector2D& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform2fv(location, 1, value.data());
}
void Shader::set_vec3(const std::string& name, const CVector3D& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform3fv(location, 1, value.data());
}
void Shader::set_vec4(const std::string& name, const CVector4D& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform4fv(location, 1, value.data());
}

void Shader::set_vec2(const std::string& name, float x, float y) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform2f(location, x, y);
}
void Shader::set_vec3(const std::string& name, float x, float y, float z) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform3f(location, x, y, z);
}
void Shader::set_vec4(const std::string& name, float x, float y, float z, float w) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniform4f(location, x, y, z, w);
}

void Shader::set_mat4(const std::string& name, const CMatrix4x4& value) {
	uint location = core->glGetUniformLocation(shader_id, name.c_str());
	core->glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}
