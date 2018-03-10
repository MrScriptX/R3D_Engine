#pragma once

#include "VkHeader.h"
#include "vertex.h"

class LoadModel
{
public:
	LoadModel();
	~LoadModel();

	void loadModel(const std::string& modelPath, const std::string& texturePath);

	std::vector<Vertex>& getVertex();
	std::vector<uint32_t>& getIndex();

private:

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
};

