#include "Mesh.h"

#ifndef TINYOBJLOADER_IMPLEMENTATION

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#endif // !TINYOBJLOADER_IMPLEMENTATION


Mesh::Mesh(const std::string& obj_path, std::shared_ptr<Renderer> p_renderer) : m_obj_path(obj_path), mp_renderer(p_renderer)
{
	loadModel();
}

Mesh::~Mesh()
{
}

void Mesh::draw(const VkCommandBuffer& command_buffer, const Pipeline& pipeline)
{
	VkBuffer vertex_buffer[] = { m_buffer.vertex };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffer, offsets);
	vkCmdBindIndexBuffer(command_buffer, m_buffer.index, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 1, &p_material->getTexture()->getDescriptorSet(), 0, nullptr);

	vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
}

void Mesh::loadModel()
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, m_obj_path.c_str()))
	{
		throw std::runtime_error(err);
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

void Mesh::bindMaterial(std::shared_ptr<Material> mat, VkBuffer& ubo, std::shared_ptr<Renderer> renderer)
{
	renderer->allocateDescriptorSet(mat->getTexture()->getDescriptorSet());
	renderer->updateDescriptorSet(ubo, mat->getTexture()->getDescriptorSet(), mat->getTexture()->getImageView(), mat->getTexture()->getSampler());

	p_material = mat;
}

void Mesh::createBuffer(std::shared_ptr<Renderer> engine)
{
	engine->createVerticesBuffer(std::make_shared<std::vector<Vertex>>(m_vertices), m_buffer);
	engine->createIndicesBuffer(std::make_shared<std::vector<uint32_t>>(m_indices), m_buffer);
}

void Mesh::destroyMesh()
{
	vkDestroyBuffer(mp_renderer->getDevice(), m_buffer.index, nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_buffer.index_memory, nullptr);

	vkDestroyBuffer(mp_renderer->getDevice(), m_buffer.vertex, nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_buffer.vertex_memory, nullptr);
}

std::vector<Vertex>& Mesh::get_vertices()
{
	return m_vertices;
}

std::vector<uint32_t>& Mesh::get_indices()
{
	return m_indices;
}

Buffer & Mesh::getBuffer()
{
	return m_buffer;
}

std::shared_ptr<Material> Mesh::getMaterial()
{
	return p_material;
}
