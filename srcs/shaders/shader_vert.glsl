#version 410

layout (location = 0) in vec3 aPos;

uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0);
	FragPos = worldPos.xyz;

	Normal = normalize(mat3(model) * vec3(0.0, 0.0, 1.0));
	gl_Position = worldPos;
}
