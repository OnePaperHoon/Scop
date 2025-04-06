#include "Material.hpp"
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

std::unordered_map<std::string, Material> MTLLoader::Load(const std::string &path)
{
	std::unordered_map<std::string, Material> materials;
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cerr << "Failed to open MTL file: " << path << std::endl;
		return materials;
	}

	std::string line;
	Material current;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "newmtl")
		{
			if (!current.name.empty())
				materials[current.name] = current;
			current = Material();
			iss >> current.name;
		}
		else if (type == "Ns")
			iss >> current.Ns;
		else if (type == "Kd")
			iss >> current.Kd.r >> current.Kd.g >> current.Kd.b;
		else if (type == "Ks")
			iss >> current.Ks.r >> current.Ks.g >> current.Ks.b;
		else if (type == "Ka")
			iss >> current.Ka.r >> current.Ka.g >> current.Ka.b;
		else if (type == "d")
			iss >> current.d;
		else if (type == "illum")
			iss >> current.illum;
		else if (type == "Ni")
			iss >> current.Ni;
		else if (type == "map_Kd")
			iss >> current.map_Kd;
	}
	if (!current.name.empty())
		materials[current.name] = current;

	return materials;
}
