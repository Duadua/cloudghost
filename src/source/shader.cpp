#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include "cdebuger.h"

IMPLEMENT_CLASS(Shader)

Shader::Shader() {
	if (glewExperimental == false) {
		glewExperimental = true;
        GLenum flag = glewInit();
		if (flag != GLEW_OK) {
			c_debuger() << "[error][glew]init fail";
            c_debuger() << reinterpret_cast<const char*>(glewGetErrorString(flag));
		}
	}
}
Shader::~Shader() { }

SPTR_Shader Shader::use() { 
	glUseProgram(shader_id);
	return shared_from_this();
}

void Shader::compile(const std::string& v_code, const std::string& f_code, const std::string& g_code) {
    uint v_id = 0, f_id = 0, g_id = 0;
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
    glShaderSource(id, 1, &t_code, nullptr);

	glCompileShader(id);
	check_compile_errors(id);

	glAttachShader(shader_id, id);
	return id;
}

void Shader::link_shaders() {
	glLinkProgram(shader_id);
	check_link_errors(shader_id);
}

// 检查错误信息
void Shader::check_compile_errors(uint id) {
	int flag;
	char res[1024];
	glGetShaderiv(id, GL_COMPILE_STATUS, &flag);
	if (!flag) {
		glGetShaderInfoLog(id, 1024, nullptr, res);
        std::string info = "[error][shader][compile]" + name + "\n";
        info.append(res);
		c_debuger() << info;
	}
}
void Shader::check_link_errors(uint id) {
	int flag;
	char res[1024];
	glGetProgramiv(id, GL_LINK_STATUS, &flag);
	if (!flag) {
		glGetProgramInfoLog(id, 1024, nullptr, res);
		std::string info = "[error][shader][link]" + name + "\n";
		info.append(res);
		c_debuger() << info;
	}
}

void Shader::set_bool(const std::string& n, const bool& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
    glUniform1i(location, value);
}
void Shader::set_int(const std::string& n, const int& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform1i(location, value);
}
void Shader::set_uint(const std::string& n, const uint& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform1ui(location, value);
}

void Shader::set_float(const std::string& n, const float& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform1f(location, value);
}

void Shader::set_vec2(const std::string& n, const CVector2D& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform2fv(location, 1, value.data());
}
void Shader::set_vec3(const std::string& n, const CVector3D& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform3fv(location, 1, value.data());
}
void Shader::set_vec4(const std::string& n, const CVector4D& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform4fv(location, 1, value.data());
}

void Shader::set_vec2(const std::string& n, float x, float y) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform2f(location, x, y);
}
void Shader::set_vec3(const std::string& n, float x, float y, float z) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform3f(location, x, y, z);
}
void Shader::set_vec4(const std::string& n, float x, float y, float z, float w) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniform4f(location, x, y, z, w);
}

void Shader::set_mat4(const std::string& n, const CMatrix4x4& value) {
    GLint location = glGetUniformLocation(shader_id, n.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}

void Shader::set_unifom_buffer(const std::string& n, uint idx) {
	uint u_block_idx = glGetUniformBlockIndex(shader_id, n.c_str());
	glUniformBlockBinding(shader_id, u_block_idx, idx);
}

// ==========================================================================================

IMPLEMENT_CLASS(ShaderStack)

SPTR_ShaderStack ShaderStack::push(const SPTR_Shader shader) {
	shaders.push(shader); use();
	return shared_from_this();
}
SPTR_ShaderStack ShaderStack::pop() {
	if (!shaders.empty()) shaders.pop(); use();
	return shared_from_this();
}

SPTR_Shader ShaderStack::top() const {
	if (shaders.empty()) return nullptr;
	return shaders.top();
}

SPTR_ShaderStack ShaderStack::use() {
	if (!shaders.empty() && shaders.top()) shaders.top()->use();
	return shared_from_this();
}




