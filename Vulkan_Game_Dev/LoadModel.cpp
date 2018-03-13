#include "LoadModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>


LoadModel::LoadModel()
{
}


LoadModel::~LoadModel()
{
}


void LoadModel::loadModel(const std::string& modelPath, const std::string& texturePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str()))
	{
		throw std::runtime_error(err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex = {};

			vertex.pos =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
				m_vertices.push_back(vertex);
			}

			m_indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void LoadModel::drawIndexed(std::vector<VkCommandBuffer>& commandBuffer)
{
	for (size_t i = 0; i < commandBuffer.size(); i++)
	{
		vkCmdDrawIndexed(commandBuffer[i], static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
	}
}



/**********************************get fonctions********************************/
/*******************************************************************************/

std::vector<Vertex>& LoadModel::getVertex()
{
	return m_vertices;
}

std::vector<uint32_t>& LoadModel::getIndex()
{
	return m_indices;
}
