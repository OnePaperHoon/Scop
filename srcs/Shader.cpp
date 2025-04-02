#include "Shader.hpp"
#include <OpenGL/OpenGL.h>
#include <fstream>
#include <sstream>

/* PUBLIC */
Shader::Shader(const char* vsPath, const char* fsPath)
{
	mProgramId = CompileShader(vsPath, fsPath);
	LoadActiveUniforms();
}

Shader::~Shader()
{
	if (mProgramId != 0)
		glDeleteProgram(mProgramId);
}

void Shader::Use()
{
	glUseProgram(mProgramId);
}

GLuint Shader::GetId()
{
	return (mProgramId);
}

GLint Shader::GetLocation(const std::string& name) const
{
	std::unordered_map<std::string, GLint>::const_iterator it = mUniformLocations.find(name);
	if (it != mUniformLocations.end())
		return (it->second);
	return (-1);
}

void	Shader::SetBool(const std::string& name, bool value) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniform1i(location, (int)value);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetInt(const std::string& name, int value) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniform1i(location, value);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetFloat(const std::string& name, float value) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniform1f(location, value);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetVec2(const std::string& name, float x, float y) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniform2f(location, x, y);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniform3f(location, x, y, z);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniform4f(location, x, y, z, w);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetMat3(const std::string& name, const float* value) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniformMatrix3fv(location, 1, GL_FALSE, value);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

void	Shader::SetMat4(const std::string& name, const float* value) const
{
	GLint location = GetLocation(name);
	if (location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	else
		std::cerr << "Not Found : " << name << " Uniform" << std::endl;
}

/* Public Utils */
static const char* GetGLTypeString(GLenum type) {
	switch (type) {
		case GL_FLOAT: return "float";
		case GL_FLOAT_VEC2: return "vec2";
		case GL_FLOAT_VEC3: return "vec3";
		case GL_FLOAT_VEC4: return "vec4";
		case GL_INT: return "int";
		case GL_INT_VEC2: return "ivec2";
		case GL_INT_VEC3: return "ivec3";
		case GL_INT_VEC4: return "ivec4";
		case GL_BOOL: return "bool";
		case GL_BOOL_VEC2: return "bvec2";
		case GL_BOOL_VEC3: return "bvec3";
		case GL_BOOL_VEC4: return "bvec4";
		case GL_FLOAT_MAT2: return "mat2";
		case GL_FLOAT_MAT3: return "mat3";
		case GL_FLOAT_MAT4: return "mat4";
		case GL_SAMPLER_2D: return "sampler2D";
		case GL_SAMPLER_CUBE: return "samplerCube";
		default: return "unknown";
	}
}

void Shader::PrintActiveUniforms() const {
	GLint count;
	glGetProgramiv(mProgramId, GL_ACTIVE_UNIFORMS, &count);

	std::cout << "Active Uniforms in Shader Program " << mProgramId << ":" << std::endl;

	for (int i = 0; i < count; ++i) {
		GLchar name[256];
		GLsizei length;
		GLint size;
		GLenum type;
		glGetActiveUniform(mProgramId, i, sizeof(name), &length, &size, &type, name);
		std::cout << " - " << name << " (Type: " << GetGLTypeString(type) << ", Size: " << size << ")" << std::endl;
	}
}

/* PRIVATE */
GLuint	Shader::CompileShader(const char* vsPath, const char* fsPath)
{
	std::string vsCode = ReadFile(vsPath);
	std::string fsCode = ReadFile(fsPath);

	if (vsCode.empty() || fsCode.empty())
	{
		std::cerr << "Error: Failed to load shader file code." << std::endl;
		return (0);
	}

	GLuint vs = AddShader(vsCode.c_str(), GL_VERTEX_SHADER);
	GLuint fs = AddShader(fsCode.c_str(), GL_FRAGMENT_SHADER);
	if (!vs || !fs)
	{
		return (0);
	}
	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, vs);
	glAttachShader(ShaderProgram, fs);
	glLinkProgram(ShaderProgram);

	GLint success;
	GLchar infoLog[1024];
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ShaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error linking shader program: " << infoLog << std::endl;
		return (0);
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return (ShaderProgram);
}

std::string Shader::ReadFile(const char* filePath)
{
	std::string		content;
	std::ifstream	fileStream(filePath, std::ios::in);

	while(!fileStream.is_open())
	{
		std::cerr << "Failed to read " << filePath << " file The file doesn't exist." << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	fileStream.close();
	return buffer.str();
}

GLuint	Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		std::cerr << "Error: Failed to create shader object." << std::endl;
		return (0);
	}
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	GLint success;
	GLchar infoLog[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		const char* typeStr = (shaderType == GL_VERTEX_SHADER) ? "VERTEX" :
								(shaderType == GL_FRAGMENT_SHADER) ? "FRAGMENT" : "UNKNOWN";

		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cerr << "[Shader Compilation Error] (" << typeStr << " SHADER):\n" << infoLog << std::endl;

		glDeleteShader(shader);
		return (0);
	}

	return shader;
}

void Shader::LoadActiveUniforms()
{
	GLint count;
	glGetProgramiv(mProgramId, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; ++i)
	{
		GLchar	name[256];
		GLsizei	length;
		GLint	size;
		GLenum	type;

		glGetActiveUniform(mProgramId, i, sizeof(name), &length, &size, &type, name);

		GLint loaction = glGetUniformLocation(mProgramId, name);
		mUniformLocations[name] = loaction;
	}
}
