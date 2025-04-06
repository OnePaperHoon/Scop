#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <float.h>
#include "MathUtils.hpp"
#include "Material.hpp"
#include "Shader.hpp"

struct Vertex {
	Vec3 position;
	Vec3 normal = { 0.0f, 0.0f, 0.0f };
	Vec2 texCoord = { 0.0f, 0.0f };
	Vec3 color = { 0.0f, 0.0f, 0.0f};
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
		void SetShader(Shader* shader);
		void UseTexture(bool value);
		void LoadTexture(const std::string& path);
		void SetScale(float scale);
		void SetRotation(float rotX, float rotY);
		void SetTranslation(float offsetX, float offsetY, float offsetZ);

		private: // Private Function
		void LoadOBJ(const char* filePath);
		void SetupMesh(void);
		void NormalizeVertices(std::vector<Vertex>& vertices);
		void ComputeNormals(void);
		void GenerateTexCoordsFromPosition(void);
		void ApplyFaceColorsFromIndex(void);
		void UpdateBlend(bool toTexture, float deltaTime);
	private: // Variable
		/**
		 * @brief
		 * 현재 3D 모델에 대한 obj 정보
		 * Vec3 Position
		 * Vec3 Normal
		 * Vec2 TextureCoord
		 */
		std::vector<Vertex> mVertices;
		/**
		 * @brief
		 * 현재 3D 모델의 요소 인덱스 정보 (EBO에 사용됨)
		 */
		std::vector<unsigned int>	mAllIndices;
		std::vector<Face>			mFaces;
		/**
		 * @brief
		 * 모델 스케일을 조절하는 비율 값
		 */
		float						mScaleFactor;
		/**
		 * @brief
		 * 모델 회전을 조절하는 값
		 */
		float						mRotX, mRotY;
		/**
		 * @brief
		 * 현재 오브젝트의 x, y, z 좌표
		 */
		float						mPosX, mPosY, mPosZ;
		/**
		 * @brief
		 * OpenGL 객체 식별자: VAO, VBO, EBO
		 */
		GLuint 						mVAO, mVBO, mEBO;

		Shader*										mShaderProgram;		// 현재 오브젝트에 대한 Shader Program ID
		GLuint										mTextureId;			// 현재 오브젝트에 대한 Texture ID
		std::unordered_map<std::string, Material>	mMaterials;			// usemtl이 여러개일 경우를 상정하여 Material node_map
		std::string									mCurrentMaterial;	// LoadOBJ & Draw 에서 currentMaterial 조정
		bool										mbUseTexture;		// 'T' 키에 대한 텍스쳐 모드 여부
		float										mBlendFactor;
	private: // TEST PRINT INFO
		void PrintAllVertexInfo();
		void PrintAllFaceInfo();
};
