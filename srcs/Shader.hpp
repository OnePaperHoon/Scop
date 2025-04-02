#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include <unordered_map>

/**
 * @brief
 *
 */
class Shader
{
	public:
		Shader(const char* vsPath, const char* fsPath);
		~Shader();

		void		Use();
		GLuint		GetId();
		GLint		GetLocation(const std::string& name) const;

		void		SetBool(const std::string& name, bool value) const;
		void		SetInt(const std::string& name, int value) const;
		void		SetFloat(const std::string& name, float value) const;
		void		SetVec2(const std::string& name, float x, float y) const;
		void		SetVec3(const std::string& name, float x, float y, float z) const;
		void		SetVec4(const std::string& name, float x, float y, float z, float w) const;
		void		SetMat3(const std::string& name, const float* value) const;
		void		SetMat4(const std::string& name, const float* value) const;

		/**
		 * @brief
		 * Print Terminal Shader Uniform Info
		 */
		void		PrintActiveUniforms() const;
	private:
		GLuint		CompileShader(const char* vsPath, const char* fsPath);
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
		void		LoadActiveUniforms();

	private:
		GLuint		mProgramId;
		/**
		 * @brief
		 * In Shader Program Active Uniforms
		 */
		std::unordered_map<std::string, GLint> mUniformLocations;
};
