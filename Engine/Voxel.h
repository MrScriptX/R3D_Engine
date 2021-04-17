#ifndef _VOXEL_H
#define _VOXEL_H

#include <memory>
#include <vector>
#include <iostream>

#include <glm\glm.hpp>


namespace Voxel
{
	const uint32_t CHUNK_SIZE = 16;
	const uint32_t CHUNK_SIZE_SQR = CHUNK_SIZE * CHUNK_SIZE;
	const float BLOCK_RENDER_SIZE = 0.5f;

	enum BlockType
	{
		basic
	};

	struct Block
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
	};
}

struct Mesh
{
	std::vector<Voxel::Block> vertices;
	std::vector<uint16_t> indices;

	int32_t nb_vertices = -1;

	uint32_t addVertex(glm::vec3 vertex, glm::vec3 color, glm::vec2 texCoord)
	{
		Voxel::Block block = { { vertex }, { color }, { texCoord } };

		vertices.push_back(block);

		nb_vertices++;

		return nb_vertices;
	}

	void addIndices(uint32_t index_x, uint32_t index_y, uint32_t index_z)
	{
		indices.push_back(index_x);
		indices.push_back(index_y);
		indices.push_back(index_z);
	}
};


#endif // !_VOXEL_H

