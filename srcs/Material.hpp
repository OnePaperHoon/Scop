#pragma once

#include <string>
#include <unordered_map>

struct RGB {
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
};

struct Material {
	std::string name;
	RGB Kd = {1.0f, 1.0f, 1.0f};		// Diffuse
	RGB Ka = {0.0f, 0.0f, 0.0f};		// Ambient
	RGB Ks = {0.0f, 0.0f, 0.0f};		// Specular

	float Ns = 0.0f;	// Shininess
	float d = 1.0f;		// Opacity
	float Ni = 1.0f;	// Index of Refraction
	int illum = 0;		// Illumination model

	std::string map_Kd;	// Diffuse map texture file name
};

class MTLLoader {
	public:
		static std::unordered_map<std::string, Material> Load(const std::string& path);
};
