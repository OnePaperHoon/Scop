#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0);
	FragPos = worldPos.xyz;

	Normal = normalize(mat3(model) * aNormal);
	TexCoord = aTexCoord;
	gl_Position = worldPos;
}
