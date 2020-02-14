#include "Model.h"


#ifndef TINYOBJLOADER_IMPLEMENTATION

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#endif // !TINYOBJLOADER_IMPLEMENTATION

Model::Model()
{
}

Model::~Model()
{
}

void Model::loadModel(const std::string& file)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex = {};

			vertex.pos = {
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

void Model::createBuffer(std::shared_ptr<Renderer> engine)
{
	engine->createVerticesBuffer(std::make_shared<std::vector<Vertex>>(m_vertices), m_buffer);
	engine->createIndicesBuffer(std::make_shared<std::vector<uint32_t>>(m_indices), m_buffer);
}

std::vector<Vertex>& Model::get_vertices()
{
	return m_vertices;
}

std::vector<uint32_t>& Model::get_indices()
{
	return m_indices;
}

Buffer & Model::get_buffer()
{
	return m_buffer;
}
