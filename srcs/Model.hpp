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
#include "Shader.hpp"

struct Vec3 {
	float x, y, z;

	bool operator==(const Vec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}
};

struct Vec2 {
	float x, y;

	bool operator==(const Vec2& other) const {
		return x == other.x && y == other.y;
	}
};

struct Vertex {
	Vec3 position;
	Vec3 normal = { 0.0f, 0.0f, 0.0f };
	Vec2 texCoord = { 0.0f, 0.0f };

	bool operator==(const Vertex& other ) const {
		return	position == other.position &&
				normal == other.normal &&
				texCoord == other.texCoord;
	}
};

struct Face {
	std::vector<unsigned int> vertexIndices;
	std::string mtlname;
};

class Model
{
	public:
		Model(const char* filePath, Shader* shader);
		~Model();

	public: // public Function
		void Draw(void);
		void SetScale(float scale);
		void SetRotation(float rotX, float rotY);
		void SetShader(Shader* shader);
		void LoadTexture(const std::string& path);

	private: // Private Function
		void LoadOBJ(const char* filePath);
		void SetupMesh(void);
		void NormalizeVertices(std::vector<Vertex>& vertices);
		void ComputeNormals(void);
		void GenerateTexCoordsFromPosition(void);
	private: // Variable
		/**
		 * @brief
		 * 현재 3D 모델에 대한 obj 정보
		 * Vec3 Position
		 * Vec3 Normal
		 * Vec2 TextureCoord
		 */
		std::vector<Vertex> vertices;
		/**
		 * @brief
		 * 현재 3D 모델의 요소 인덱스 정보 (EBO에 사용됨)
		 */
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
		GLuint										mTextureId;			// 현재 오브젝트에 대한 Texture ID
		Shader*										mShaderProgram;		// 현재 오브젝트에 대한 Shader Program ID
		std::unordered_map<std::string, Material>	materials;			// usemtl이 여러개일 경우를 상정하여 Material node_map
		std::string									currentMaterial;	// LoadOBJ & Draw 에서 currentMaterial 조정
		bool										hasTexCoord;

	private: // TEST PRINT INFO
		void PrintAllVertexInfo();
		void PrintAllFaceInfo();
};
