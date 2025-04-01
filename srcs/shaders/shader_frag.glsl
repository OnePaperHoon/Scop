#version 410

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos = vec3(0.0, 0.0, -4.0);
uniform vec3 lightPos = vec3(3.0, 3.0, -3.0);

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

void main()
{
	vec3 normal = normalize(Normal); // 고정 노멀
	vec3 fragPos = FragPos;

	vec3 lightColor = vec3(1.0); // 밝은 조명
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 ambient = material.ambient * lightColor * 1.5;
	float diff = max(dot(normal, lightDir), 0.0);
	diff = max(diff, 0.2);
	vec3 diffuse = diff * material.diffuse * lightColor;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * material.specular * lightColor;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
