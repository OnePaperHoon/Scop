#include <_stdio.h>
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
, hasTexCoord(false)
{
	LoadOBJ(filePath);
	SetupMesh();
	LoadTexture("resources/sample.png"); // TEST
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
	std::vector<Vec3> temp_V;
	std::vector<Vec2> temp_Vt;
	std::vector<Vec3> temp_Vn;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;
		// mtl 파일 파싱
		if (prefix == "mtllib")
		{
			std::string mtlFilename;
			iss >> mtlFilename;
			materials = MTLLoader::Load("resources/" + mtlFilename);
			std::cout << mtlFilename << std::endl;
		}
		else if (prefix == "v") // v 1.0123, 2.123123, -6.1235 -> 정점 좌표 정보일 경우
		{
			Vec3 v;
			iss >> v.x >> v.y >> v.z;
			temp_V.push_back(v);
		}
		else if (prefix == "vt") // vt 0~1, 0~1 -> TextureCoord일 경우
		{
			Vec2 vt;
			iss >> vt.x >> vt.y;
			temp_Vt.push_back(vt);
			hasTexCoord = true;
		}
		else if (prefix == "vn") // vn 1, 2, 5 -> Normal 정보일 경우
		{
			Vec3 vn;
			iss >> vn.x >> vn.y >> vn.z;
			temp_Vn.push_back(vn);
		}
		else if (prefix == "usemtl") // v, vn, vt 정보가 모두 처리 완료 된 후 Usemtl -> 이후 f들에게 적용할 mtl 명
		{
			iss >> currentMaterial;
		}
		else if (prefix == "f") // f 정보가 나오면
		{
			Face face;
			face.mtlname = currentMaterial;
			std::string vertexStr;

			static std::unordered_map<std::string, unsigned int> vertexCache;

			while (iss >> vertexStr)
			{
				std::cout << vertexStr << std::endl;
				if (vertexCache.count(vertexStr))
				{
					face.vertexIndices.push_back(vertexCache[vertexStr]);
					continue;
				}

				int vIdx = -1, vtIdx = -1, vnIdx = -1;
				size_t firstSlash = vertexStr.find('/'); // f 1/2/3 에서 첫번째 슬레쉬 찾기
				size_t secondSlash = vertexStr.find('/', firstSlash + 1); // 두번째 슬레쉬 찾기

				if (firstSlash == std::string::npos)
				{
					vIdx = std::stoi(vertexStr) - 1;
				}
				else if (secondSlash == std::string::npos)
				{
					vIdx = std::stoi(vertexStr.substr(0, firstSlash)) - 1;
					vtIdx = std::stoi(vertexStr.substr(firstSlash + 1)) - 1;
				}
				else if (secondSlash == firstSlash + 1)
				{
					vIdx = std::stoi(vertexStr.substr(0, firstSlash)) - 1;
					vnIdx = std::stoi(vertexStr.substr(secondSlash + 1)) - 1;
				}
				else
				{
					vIdx = std::stoi(vertexStr.substr(0, firstSlash)) - 1;
					vtIdx = std::stoi(vertexStr.substr(firstSlash + 1, secondSlash - firstSlash - 1)) - 1;
					vnIdx = std::stoi(vertexStr.substr(secondSlash + 1)) - 1;
				}

				Vertex v;
				v.position = temp_V[vIdx];
				if (vtIdx >= 0 && temp_Vt.size() != 0)
					v.texCoord = temp_Vt[vtIdx];
				if (vnIdx >= 0 && temp_Vn.size() != 0)
					v.normal = temp_Vn[vnIdx];

				vertices.push_back(v);
				unsigned int index = vertices.size() - 1;
				vertexCache[vertexStr] = index;
				face.vertexIndices.push_back(index);
			}
			if (face.vertexIndices.size() == 3)
			{
				faces.push_back(face);
			}
			else if (face.vertexIndices.size() == 4)
			{
				Face f1, f2;
				f1.mtlname = face.mtlname;
				f1.vertexIndices = { face.vertexIndices[0], face.vertexIndices[1], face.vertexIndices[2] };

				f2.mtlname = face.mtlname;
				f2.vertexIndices = { face.vertexIndices[0], face.vertexIndices[2], face.vertexIndices[3] };

				faces.push_back(f1);
				faces.push_back(f2);
			}
		}
	}
	file.close();
	NormalizeVertices(vertices); // NDC 좌표계로
	if (!hasTexCoord)
	{
		GenerateTexCoordsFromPosition();
	}
	PrintAllVertexInfo();
	// PrintAllFaceInfo();
	// 정점 및 인덱스 저장
}

void Model::SetupMesh(void)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	/*
		Vertex
		->	float * 3 : position
			float * 3 : normal
			float * 2 : texcoord
	*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// 각 face의 인덱스를 모아 하나의 인덱스 버퍼 생성
	allIndices.clear();
	for (const Face& face : faces) {
		allIndices.insert(allIndices.end(), face.vertexIndices.begin(), face.vertexIndices.end());
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int), allIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Model::NormalizeVertices(std::vector<Vertex>& vertices)
{
	float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;

	// 1. 최소/최대 좌표 계산
	for (const auto& v : vertices)
	{
		minX = std::min(minX, v.position.x);
		minY = std::min(minY, v.position.y);
		minZ = std::min(minZ, v.position.z);

		maxX = std::max(maxX, v.position.x);
		maxY = std::max(maxY, v.position.y);
		maxZ = std::max(maxZ, v.position.z);
	}

	// 2. 중심 및 최대 범위 계산
	float centerX = (maxX + minX) / 2.0f;
	float centerY = (maxY + minY) / 2.0f;
	float centerZ = (maxZ + minZ) / 2.0f;
	float maxRange = std::max({ maxX - minX, maxY - minY, maxZ - minZ });

	// 3. 정규화 (NDC 범위 맞춤)
	for (auto& v : vertices)
	{
		v.position.x = (v.position.x - centerX) / maxRange;
		v.position.y = (v.position.y - centerY) / maxRange;
		v.position.z = (v.position.z - centerZ) / maxRange;
	}
}

void Model::GenerateTexCoordsFromPosition(void)
{
	std::cout << "🌀 No texCoord found — Generating UVs from position.\n";

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].texCoord.x = (vertices[i].position.x + 1.0f) * 0.5f;
		vertices[i].texCoord.y = (vertices[i].position.y + 1.0f) * 0.5f;
	}
}


/* ----------------TEST---------------- */
void Model::PrintAllVertexInfo()
{
	std::vector<Vertex>::const_iterator it = vertices.begin();
	for (; it != vertices.end(); it++)
	{
		std::cout << "-----------------Vertices Info---------------" << std::endl;
		std::cout << "postiton| x : " << it->position.x << " y : "<< it->position.y << " z : " << it->position.z << std::endl;
		std::cout << "normal  | x : " << it->normal.x << " y : " << it->normal.y << " z : " <<  it->normal.z <<std::endl;
		std::cout << "texCoord| x : " << it->texCoord.x << " y : " << it->texCoord.y <<std::endl;
	}
}

void Model::PrintAllFaceInfo()
{
	std::vector<Face>::const_iterator it = faces.begin();
	for (; it != faces.end(); it++)
	{
		std::cout << "----------------Face Info-----------------" << std::endl;
		std::cout << "vertexIndces: ";
		for (std::vector<unsigned int>::const_iterator index = it->vertexIndices.begin(); index != it->vertexIndices.end(); index++)
		{
			std::cout << *index << " ";
		}
		std::cout << std::endl;
		std::cout << "mtlName : " << it->mtlname << std::endl;
	}
}
