#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "loader.h"

#include <GLFW/glfw3.h>

IMPLEMENT_CLASS(Shader)

Shader::Shader() {
	/*if (glewExperimental == false) {
		glewExperimental = true;
		glfwInit();
	}*/
}
Shader::~Shader() { }

SPTR_Shader Shader::use() { 
	glUseProgram(shader_id);
	return shared_from_this();
}

void Shader::compile(const std::string& v_code, const std::string& f_code, const std::string& g_code) {
	uint v_id, f_id, g_id;
	shader_id = glCreateProgram();
	
	// compile vertex shader
	v_id = compile_shader(v_code, GL_VERTEX_SHADER);

	// compile fragment shader
	f_id = compile_shader(f_code, GL_FRAGMENT_SHADER);

	// compile geometry shader
	if (g_code.compare("") != 0) { g_id = compile_shader(g_code, GL_GEOMETRY_SHADER); }

	link_shaders();

	glDeleteShader(v_id);
	glDeleteShader(f_id);
	if(g_code.compare("") != 0) glDeleteShader(g_id);
	
}

uint Shader::compile_shader(const std::string& code, uint type) {
	const char* t_code = code.c_str();
	uint id = glCreateShader(type);
	glShaderSource(id, 1, &t_code, NULL);

	glCompileShader(id);
	check_compile_errors(id);

	glAttachShader(shader_id, id);
	return id;
}

void Shader::link_shaders() {
	glLinkProgram(shader_id);
	check_link_errors(shader_id);
}

void Shader::check_compile_errors(uint id) {
	int flag;
	char res[1024];
	glGetShaderiv(id, GL_COMPILE_STATUS, &flag);
	if (!flag) {
		glGetShaderInfoLog(id, 1024, NULL, res);
		std::string info = "【error】【shader】【compile】\n";
		info.append(res);
		c_debug() << info;
	}
}
void Shader::check_link_errors(uint id) {
	int flag;
	char res[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &flag);
	if (!flag) {
		glGetProgramInfoLog(id, 1024, NULL, res);
		std::string info = "【error】【shader】【link】\n";
		info.append(res);
		c_debug() << info;
	}
}

void Shader::set_bool(const std::string& name, const bool& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform1i(location, (int)value);
}
void Shader::set_int(const std::string& name, const int& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform1i(location, value);
}
void Shader::set_uint(const std::string& name, const uint& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform1ui(location, value);
}

void Shader::set_float(const std::string& name, const float& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform1f(location, value);
}

void Shader::set_vec2(const std::string& name, const CVector2D& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform2fv(location, 1, value.data());
}
void Shader::set_vec3(const std::string& name, const CVector3D& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform3fv(location, 1, value.data());
}
void Shader::set_vec4(const std::string& name, const CVector4D& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform4fv(location, 1, value.data());
}

void Shader::set_vec2(const std::string& name, float x, float y) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform2f(location, x, y);
}
void Shader::set_vec3(const std::string& name, float x, float y, float z) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform3f(location, x, y, z);
}
void Shader::set_vec4(const std::string& name, float x, float y, float z, float w) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform4f(location, x, y, z, w);
}

void Shader::set_mat4(const std::string& name, const CMatrix4x4& value) {
	uint location = glGetUniformLocation(shader_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}
