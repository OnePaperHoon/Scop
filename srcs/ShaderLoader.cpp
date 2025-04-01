#include "ShaderLoader.hpp"
#include <OpenGL/OpenGL.h>
#include <string>


ShaderLoader::ShaderLoader()
{}

ShaderLoader::~ShaderLoader()
{}


std::string ShaderLoader::ReadFile(const char* filePath)
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

GLuint	ShaderLoader::AddShader(const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
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
		return (0);
	}

	return shader;
}

GLuint	ShaderLoader::CompileShader(const char* vsPath, const char* fsPath)
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
