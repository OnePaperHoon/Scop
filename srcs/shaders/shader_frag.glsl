#version 410

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 VertexColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform sampler2D texture1;
uniform float blendFactor;

void main()
{
	vec3 texColor = texture(texture1, TexCoord).rgb * material.diffuse;
	vec3 finalColor = mix(VertexColor, texColor, blendFactor);

	vec3 result = material.ambient + finalColor;
	FragColor = vec4(result, 1.0);
}
