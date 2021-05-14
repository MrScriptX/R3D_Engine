#include "Mesh.h"

Mesh::Mesh(const int ID, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Renderer> p_renderer) : m_ID(ID), m_vertices(vertices), m_indices(indices), mp_renderer(p_renderer)
{
	m_buffer.fill({VK_NULL_HANDLE});
	m_old_buffer.fill({ VK_NULL_HANDLE });
	m_destroyed = false;
}

Mesh::Mesh(const int ID, const std::string& obj_path, std::shared_ptr<Renderer> p_renderer) : m_ID(ID), m_obj_path(obj_path), mp_renderer(p_renderer)
{
	m_buffer.fill({ VK_NULL_HANDLE });
	m_old_buffer.fill({ VK_NULL_HANDLE });
	m_destroyed = false;

	loadModel();
}

Mesh::~Mesh()
{
	std::clog << "clear mesh" << std::endl;
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		DestroyBuffers(i);
	}
}

void Mesh::draw(const VkCommandBuffer& command_buffer, const int32_t frame)
{
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_material->GetPipeline().handle);

	VkBuffer vertex_buffer[] = { m_buffer[frame].vertex };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffer, offsets);
	vkCmdBindIndexBuffer(command_buffer, m_buffer[frame].index, 0, VK_INDEX_TYPE_UINT32);

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

void Mesh::Delete()
{
	m_destroyed = true;
}

void Mesh::CreateBuffers(std::shared_ptr<Renderer> engine)
{
	m_old_buffer = m_buffer;

	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		engine->createVerticesBuffer(std::make_shared<std::vector<Vertex>>(m_vertices), m_buffer[i]);
		engine->createIndicesBuffer(std::make_shared<std::vector<uint32_t>>(m_indices), m_buffer[i]);
	}
}

void Mesh::DestroyOldBuffers()
{
	for (size_t i = 0; i < m_old_buffer.size(); i++)
	{
		vkDestroyBuffer(mp_renderer->getDevice(), m_old_buffer[i].index, nullptr);
		vkFreeMemory(mp_renderer->getDevice(), m_old_buffer[i].index_memory, nullptr);

		vkDestroyBuffer(mp_renderer->getDevice(), m_old_buffer[i].vertex, nullptr);
		vkFreeMemory(mp_renderer->getDevice(), m_old_buffer[i].vertex_memory, nullptr);

		m_old_buffer[i].index = VK_NULL_HANDLE;
		m_old_buffer[i].index_memory = VK_NULL_HANDLE;
		m_old_buffer[i].vertex = VK_NULL_HANDLE;
		m_old_buffer[i].vertex_memory = VK_NULL_HANDLE;
	}
}

void Mesh::DestroyBuffers(const int32_t frame)
{
	vkDestroyBuffer(mp_renderer->getDevice(), m_buffer[frame].index, nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_buffer[frame].index_memory, nullptr);

	vkDestroyBuffer(mp_renderer->getDevice(), m_buffer[frame].vertex, nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_buffer[frame].vertex_memory, nullptr);

	m_buffer[frame].index = VK_NULL_HANDLE;
	m_buffer[frame].index_memory = VK_NULL_HANDLE;
	m_buffer[frame].vertex = VK_NULL_HANDLE;
	m_buffer[frame].vertex_memory = VK_NULL_HANDLE;
}

void Mesh::SetVertices(const std::vector<Vertex>& vertices)
{
	m_vertices = vertices;
}

void Mesh::SetIndices(const std::vector<uint32_t>& indices)
{
	m_indices = indices;
}

std::vector<Vertex>& Mesh::get_vertices()
{
	return m_vertices;
}

std::vector<uint32_t>& Mesh::get_indices()
{
	return m_indices;
}

Buffer& Mesh::GetBuffer(const int32_t frame)
{
	return m_buffer[frame];
}

Buffer& Mesh::GetOldBuffer(const int32_t frame)
{
	return m_old_buffer[frame];
}

std::shared_ptr<Material> Mesh::getMaterial()
{
	return p_material;
}

const bool Mesh::IsDestroyed()
{
	return m_destroyed;
}

const bool Mesh::IsCleaned()
{
	bool result = true;
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		result = result && (m_buffer[i].vertex == VK_NULL_HANDLE);
	}

	return result;
}
