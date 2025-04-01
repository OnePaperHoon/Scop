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

/*
	TODO:
		1. obj파일 안에서 mtl파일을 사용한다 (mtllib file name) 이면 mtl 파일을 파싱해야함
		2. obj 파일에서 usemtl이 나왔을 경우 그 하위 페이스 들은 usemtl name mtl파일인 name을 가지고 있을 필요가 있음
		3. 그렇다면
		struct Face {
			std::vector<unsigned int> vertexIndices;
			std::string mtlname;
		};
		형식으로 가져가고
		std::vector<Face> faces;로 가져가는게 맞을거 같은데
	구현 해야할 것
		1. LoadOBJ에서
*/

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
