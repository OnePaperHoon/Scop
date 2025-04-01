#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <float.h>
#include "Material.hpp"
#include "MathUtils.hpp"

struct Face {
	std::vector<unsigned int> vertexIndices;
	std::string mtlname;
};

class Model
{
	public:
		Model(const char* filePath, GLuint shader);
		~Model();

		void Draw(void);
		void SetScale(float scale);
		void SetRotation(float rotX, float rotY);

	private:
		void LoadOBJ(const char* filePath);
		void SetupMesh(void);
		void NormalizeVertices(std::vector<float>& vertices);
		void ComputeNormals(void);
		/**
		 * @brief
		 * 현재 3D 모델에 대한 정점 정보 (x, y, z 좌표 순서로 저장됨)
		 */
		std::vector<float>			vertices;
		/**
		 * @brief
		 * 현재 3D 모델의 요소 인덱스 정보 (EBO에 사용됨)
		 */
		std::vector<unsigned int>	indices;
		std::vector<unsigned int>	allIndices;
		std::vector<Face>			faces;
		/**
		 * @brief
		 * 모델 스케일을 조절하는 비율 값
		 */
		float						scaleFactor;
		/**
		 * @brief
		 * 모델 회전을 조절하는 값
		 */
		float						rotX, rotY;
		/**
		 * @brief
		 * OpenGL 객체 식별자: VAO, VBO, EBO, 색상 VBO
		 */
		GLuint 						VAO, VBO, EBO, colorVBO;
		/**
		 * @brief
		 * 이 모델이 사용하는 셰이더 프로그램 ID
		 */
		GLuint 						shaderProgram;
		std::unordered_map<std::string, Material>	materials;
		std::string									currentMaterial;

};
