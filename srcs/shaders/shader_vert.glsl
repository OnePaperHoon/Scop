#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec3 VertexColor;

void main()
{
	vec4 worldPos = projection * view * model * vec4(aPos, 1.0);
	FragPos = worldPos.xyz;

	Normal = normalize(mat3(model) * aNormal);

	TexCoord = aTexCoord;
	VertexColor = aColor;
	gl_Position = worldPos;
}
