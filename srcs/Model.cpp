#define STB_IMAGE_IMPLEMENTATION
#include "Model.hpp"
#include <OpenGL/gl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "library/stb_image.h"
#include "Material.hpp"

Model::Model(const char* filePath, Shader* shader)
: mShaderProgram(shader)
, scaleFactor(1.0f)
, rotX(0.0f)
, rotY(0.0f)
{
	LoadOBJ(filePath);
	SetupMesh();
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Model::Draw(void)
{
	mShaderProgram->Use();
	glBindVertexArray(VAO);

	float model[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	rotateY(rotY, model);
	rotateX(rotX, model);

	float scaleMatrix[16] = {
		scaleFactor, 0.0f, 0.0f, 0.0f,
		0.0f, scaleFactor, 0.0f, 0.0f,
		0.0f, 0.0f, scaleFactor, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	multiplyMatrix(model, scaleMatrix, model);
	mShaderProgram->SetMat4("model", model);

	size_t indexOffset = 0;
	for (const Face& face : faces)
	{
		const auto it = materials.find(face.mtlname);
		if (it != materials.end())
		{
			const Material& mat = it->second;
			mShaderProgram->SetVec3("material.diffuse", mat.Kd.r, mat.Kd.g, mat.Kd.b);
			mShaderProgram->SetVec3("material.ambient", mat.Ka.r, mat.Ka.g, mat.Ka.b);
			mShaderProgram->SetVec3("material.specular", mat.Ks.r, mat.Ks.g, mat.Ks.b);
			mShaderProgram->SetFloat("material.shininess", mat.Ns);
		}
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(indexOffset * sizeof(unsigned int)));
		indexOffset += 3;
	}
	float timeValue = glfwGetTime();
	float grrenValue = (sin(timeValue) / 2.0f + 0.5f);
	mShaderProgram->SetVec4("TimeColor", 0.0f, grrenValue, 0.0f, 1.0f);
	if (allIndices.size() % 3 != 0) std::cerr << "⚠️ Broken EBO!" << std::endl;
	glBindVertexArray(0);
}

void Model::SetScale(float scale) {
	if (scale != scaleFactor)
		std::cout << "Before Scale: " << scaleFactor << " After Scale: " << scale << std::endl;
	this->scaleFactor = scale;
}

void Model::SetRotation(float rotX, float rotY)
{
	this->rotX = rotX;
	this->rotY = rotY;
}

void Model::SetShader(Shader* shader)
{
	this->mShaderProgram = shader;
}

void Model::LoadTexture(const std::string& path)
{
	glGenTextures(1, &mTextureId);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		GLenum format = GL_RGB;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Failed to load texture: " << path << " — using fallback texture." << std::endl;

		unsigned char fallbackData[] = { 255, 0, 255, 255 }; // RGBA: magenta
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

// Private
void Model::LoadOBJ(const char* filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
		return;
	}

	std::vector<float> temp_vertices;
	std::vector<unsigned int> vertexIndices;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "mtllib")
		{
			std::string mtlFilename;
			iss >> mtlFilename;
			materials = MTLLoader::Load("resources/" + mtlFilename);
			std::cout << mtlFilename << std::endl;
		}
		else if (prefix == "v")
		{
			float x, y, z;
			iss >> x >> y >> z;
			temp_vertices.push_back(x);
			temp_vertices.push_back(y);
			temp_vertices.push_back(z);
		}
		else if (prefix == "usemtl")
		{
			iss >> currentMaterial;
		}
		else if (prefix == "f")
		{
			std::vector<unsigned int> indices;
			std::string vertex;

			// f 라인의 모든 숫자 읽기
			while (iss >> vertex)
			{
				int vIndex;
				std::istringstream vss(vertex);
				vss >> vIndex;
				indices.push_back(vIndex - 1); // OBJ 인덱스는 1부터 시작 (-1)
			}
			if (indices.size() == 3)
			{
				Face face;

				face.mtlname = currentMaterial;
				face.vertexIndices = indices;
				faces.push_back(face);
			}
			// 사각형 (4개의 정점 → 삼각형 2개로 변환)
			else if (indices.size() == 4)
			{
				Face f1, f2;

				// 첫 번째 삼각형
				f1.mtlname = currentMaterial;
				f1.vertexIndices = { indices[0], indices[1], indices[2] };
				// 두 번째 삼각형
				f2.mtlname = currentMaterial;
				f2.vertexIndices = { indices[0], indices[2], indices[3] };

				faces.push_back(f1);
				faces.push_back(f2);
			}
		}
	}
	file.close();
	NormalizeVertices(temp_vertices); // NDC 좌표계로

	// 정점 및 인덱스 저장
	vertices = temp_vertices;
	indices = vertexIndices;
}

void Model::SetupMesh(void)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// 각 face의 인덱스를 모아 하나의 인덱스 버퍼 생성
	allIndices.clear();
	for (const Face& face : faces) {
		allIndices.insert(allIndices.end(), face.vertexIndices.begin(), face.vertexIndices.end());
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int), allIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Model::NormalizeVertices(std::vector<float>& vertices)
{
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

	// 모델의 최소/최대 좌표 찾기
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		minX = std::min(minX, vertices[i]);
		minY = std::min(minY, vertices[i + 1]);
		minZ = std::min(minZ, vertices[i + 2]);

		maxX = std::max(maxX, vertices[i]);
		maxY = std::max(maxY, vertices[i + 1]);
		maxZ = std::max(maxZ, vertices[i + 2]);
	}

	// 모델의 최대 크기 구하기 (가장 긴 축을 기준으로 정규화)
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float rangeZ = maxZ - minZ;
	float maxRange = std::max(rangeX, std::max(rangeY, rangeZ));  // 가장 긴 축 찾기

	float centerX = (maxX + minX) / 2.0f;
	float centerY = (maxY + minY) / 2.0f;
	float centerZ = (maxZ + minZ) / 2.0f;

	// 모델을 중심으로 이동 후, 비율 유지하면서 `-1 ~ 1` 정규화
	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		vertices[i]   = ((vertices[i]   - centerX) / maxRange) * 1.0f;
		vertices[i+1] = ((vertices[i+1] - centerY) / maxRange) * 1.0f;
		vertices[i+2] = ((vertices[i+2] - centerZ) / maxRange) * 1.0f;
	}
}
