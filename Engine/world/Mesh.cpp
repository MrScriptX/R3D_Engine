#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, std::shared_ptr<Renderer> p_renderer) : m_vertices(vertices), m_indices(indices), mp_renderer(p_renderer)
{
}

Mesh::Mesh(const std::string& obj_path, std::shared_ptr<Renderer> p_renderer) : m_obj_path(obj_path), mp_renderer(p_renderer)
{
	loadModel();
}

Mesh::~Mesh()
{
}

void Mesh::draw(const VkCommandBuffer& command_buffer)
{
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_material->GetPipeline().handle);

	VkBuffer vertex_buffer[] = { m_buffer.vertex };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffer, offsets);
	vkCmdBindIndexBuffer(command_buffer, m_buffer.index, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_material->GetPipeline().layout, 0, 1, &p_material->getDescriptorSet(), 0, nullptr);

	vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
}

void Mesh::loadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(m_obj_path, aiProcess_Triangulate);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		for (int t = 0; t < mesh->mNumVertices; t++)
		{
			Vertex vertex = {};

			const aiVector3D* pPos = &(mesh->mVertices[t]);
			const aiVector3D* pTexCoord = &(mesh->mTextureCoords[0][t]);

			vertex.pos = {
				pPos->x,
				pPos->y,
				pPos->z
			};

			vertex.texCoord = {
				pTexCoord->x,
				1.0f - pTexCoord->y
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			m_vertices.push_back(vertex);
		}

		for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
		{
			aiFace face = mesh->mFaces[t];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				m_indices.push_back(face.mIndices[j]);
			}
		}
	}
}

void Mesh::bindMaterial(std::shared_ptr<Material> mat, VkBuffer& ubo, std::shared_ptr<Renderer> renderer)
{
	renderer->allocateDescriptorSet(mat->getDescriptorSet());

	if (mat->getTexture() == nullptr)
	{
		renderer->updateDescriptorSet(ubo, mat->getDescriptorSet());
	}
	else
	{
		renderer->updateDescriptorSet(ubo, mat->getDescriptorSet(), mat->getTexture()->getImageView(), mat->getTexture()->getSampler());
	}

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

void Mesh::SetVertices(std::vector<Vertex> vertices)
{
	m_vertices = vertices;
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
