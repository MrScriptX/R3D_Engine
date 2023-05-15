#include "Mesh.h"

#include "../renderer/vpipeline.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Renderer> p_renderer) : m_vertices(vertices), m_indices(indices), mp_renderer(p_renderer)
{
	m_buffer.fill({ VK_NULL_HANDLE });
	m_to_update.set();
}

Mesh::Mesh(const std::string& obj_path, std::shared_ptr<Renderer> p_renderer) : m_obj_path(obj_path), mp_renderer(p_renderer)
{
	m_buffer.fill({ VK_NULL_HANDLE });
	m_to_update.reset();

	loadModel();
}

Mesh::~Mesh()
{
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		DestroyBuffers(i);
	}
}

void Mesh::draw(const VkCommandBuffer& command_buffer, VkDescriptorSet& descriptorset, const int32_t frame, const std::unordered_map<std::string, vred::renderer::ipipeline>& pipelines)
{
	vred::renderer::ipipeline pipeline = pipelines.find(p_material->pipeline())->second;
	
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle);

	const VkBuffer vertex_buffer[] = { m_buffer[frame].vertex };
	const VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffer, offsets);
	vkCmdBindIndexBuffer(command_buffer, m_buffer[frame].index, 0, VK_INDEX_TYPE_UINT32);

	const VkDescriptorSet sets[] = { p_material->getDescriptorSet(), descriptorset };

	vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout, 0, 2, sets, 0, nullptr);
	// vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_material->GetPipeline().layout, 0, 1, , 0, nullptr);

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
			const aiVector3D* pNormal = &(mesh->mNormals[t]);
			const aiVector3D* pTexCoord = &(mesh->mTextureCoords[0][t]);

			vertex.pos = { pPos->x, pPos->y, pPos->z };
			vertex.normal = { pNormal->x, pNormal->y, pNormal->z };
			vertex.texCoord = { pTexCoord->x, 1.0f - pTexCoord->y };
			vertex.color = { std::sin(t), std::cos(t), 1.0f };

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

	m_to_update.set();
}

void Mesh::bindMaterial(std::shared_ptr<Material> mat, VkBuffer& ubo, std::shared_ptr<Renderer> renderer)
{
	if (mat->getTexture() == nullptr)
	{
		renderer->updateDescriptorSet(ubo, mat->getDescriptorSet(), sizeof(UniformBufferObject));
	}
	else
	{
		renderer->updateDescriptorSet(ubo, mat->getDescriptorSet(), mat->getTexture()->getImageView(), mat->getTexture()->getSampler());
	}

	p_material = mat;
}

void Mesh::CreateBuffers(std::shared_ptr<Renderer> engine)
{
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		engine->CreateVerticesBuffer(std::make_shared<std::vector<Vertex>>(m_vertices), m_buffer[i]);
		engine->CreateIndicesBuffer(std::make_shared<std::vector<uint32_t>>(m_indices), m_buffer[i]);
	}

	m_to_update.reset();
}

void Mesh::UpdateBuffers(const int32_t frame)
{
	DestroyBuffers(frame);

	mp_renderer->CreateVerticesBuffer(std::make_shared<std::vector<Vertex>>(m_vertices), m_buffer[frame]);
	mp_renderer->CreateIndicesBuffer(std::make_shared<std::vector<uint32_t>>(m_indices), m_buffer[frame]);

	m_to_update.set(frame, false);
}

void Mesh::DestroyBuffers(const int32_t frame)
{
	vkDestroyBuffer(mp_renderer->GetDevice(), m_buffer[frame].index, nullptr);
	vkFreeMemory(mp_renderer->GetDevice(), m_buffer[frame].index_memory, nullptr);

	vkDestroyBuffer(mp_renderer->GetDevice(), m_buffer[frame].vertex, nullptr);
	vkFreeMemory(mp_renderer->GetDevice(), m_buffer[frame].vertex_memory, nullptr);

	m_buffer[frame].index = VK_NULL_HANDLE;
	m_buffer[frame].index_memory = VK_NULL_HANDLE;
	m_buffer[frame].vertex = VK_NULL_HANDLE;
	m_buffer[frame].vertex_memory = VK_NULL_HANDLE;
}

void Mesh::SetVertices(const std::vector<Vertex>& vertices)
{
	m_vertices = vertices;

	m_to_update.set();
}

void Mesh::SetIndices(const std::vector<uint32_t>& indices)
{
	m_indices = indices;

	m_to_update.set();
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

std::shared_ptr<Material> Mesh::getMaterial()
{
	return p_material;
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

const bool Mesh::IsUpdated()
{
	return m_to_update == false;
}
