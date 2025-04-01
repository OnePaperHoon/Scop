#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

/**
 * @brief
 *
 */
class ShaderLoader
{
	public:
		ShaderLoader();
		~ShaderLoader();

		/**
		 * @brief
		 * 외부에서 최종적으로 엔트리 및 리턴 포인트
		 * @param vsPath Vertex Shader File Path
		 * @param fsPath Fragment Shader FIle Path
		 * @return GLuint ShaderProgram
		 */
		GLuint		CompileShader(const char* vsCode, const char* fsPath);


	private:
		/**
		 * @brief
		 * Shader File을 읽고 Code 를 리턴하는 함수
		 * @param filePath Shader File Path
		 * @return std::string Shader File Code
		 */
		std::string	ReadFile(const char* filePath);

		/**
		 * @brief
		 * 셰이더를 생성하고 컴파일하는 함수
		 * @param shaderCode Shader Code
		 * @param shaderType Shader Type
		 * @return GLuint ShaderProgram
		 */
		GLuint		AddShader(const char* shaderCode, GLenum shaderType);
};
