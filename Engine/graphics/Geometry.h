#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include "Vertex.h"

struct Voxel
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	int32_t nb_vertices = -1;

	uint32_t addVertex(glm::vec3 pos, glm::vec3 color, glm::vec2 texCoord)
	{
		Vertex vertex;
		vertex.pos = pos; 
		vertex.color = color;
		vertex.texCoord = texCoord;

		vertices.push_back(vertex);

		nb_vertices++;

		return nb_vertices;
	}

	void addIndices(uint32_t x, uint32_t y, uint32_t z)
	{
		indices.push_back(x);
		indices.push_back(y);
		indices.push_back(z);
	}
};

#endif