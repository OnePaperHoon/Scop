#version 410

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;
uniform vec4 TimeColor;
uniform sampler2D texture1;

void main()
{
	vec3 result = material.ambient + texture(texture1, TexCoord).rgb * material.diffuse + material.shininess;
	FragColor = vec4(result, 1.0) + TimeColor;
}
