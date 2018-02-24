#pragma once

#include <fstream>
#include <vector>

class ShaderLoader
{
public:
	ShaderLoader();
	~ShaderLoader();

	std::vector<char> loadFromFile(const std::string& filename);
};

