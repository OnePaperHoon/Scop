#version 410

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
uniform vec4 TimeColor;

void main()
{
	vec3 result = material.ambient + material.diffuse + material.shininess;
	FragColor = vec4(result, 1.0) + TimeColor;
}
