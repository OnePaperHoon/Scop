#include "Model.hpp"
#include "Material.hpp"

Model::Model(const char* filePath, GLuint shader)
: shaderProgram(shader)
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
	glUseProgram(shaderProgram);
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

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
			materials = MTLLoader::Load(mtlFilename);
			std::cout << mtlFilename << std::endl;
		}
		if (prefix == "v")
		{
			float x, y, z;
			iss >> x >> y >> z;
			temp_vertices.push_back(x);
			temp_vertices.push_back(y);
			temp_vertices.push_back(z);
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
				vertexIndices.push_back(indices[0]);
				vertexIndices.push_back(indices[1]);
				vertexIndices.push_back(indices[2]);
			}
			// 사각형 (4개의 정점 → 삼각형 2개로 변환)
			else if (indices.size() == 4)
			{
				// 첫 번째 삼각형
				vertexIndices.push_back(indices[0]);
				vertexIndices.push_back(indices[1]);
				vertexIndices.push_back(indices[2]);

				// 두 번째 삼각형
				vertexIndices.push_back(indices[0]);
				vertexIndices.push_back(indices[2]);
				vertexIndices.push_back(indices[3]);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

