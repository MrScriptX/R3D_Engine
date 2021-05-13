#include "GameObject.h"


GameObject::GameObject(std::shared_ptr<Renderer> p_renderer) : mp_renderer(p_renderer)
{
	m_ubo.fill(VK_NULL_HANDLE);
	m_ubo_memory.fill(VK_NULL_HANDLE);

	m_position = { 0, 0, 0 };
	m_rotation = { 0, 0, 0 };

	for (size_t i = 0; i < m_ubo.size(); i++)
	{
		mp_renderer->createUBO(m_ubo[i], m_ubo_memory[i]);
	}
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_meshes.erase(m_meshes.begin() + i);
	}

	for (size_t i = 0; i < m_ubo.size(); i++)
	{
		vkDestroyBuffer(mp_renderer->getDevice(), m_ubo[i], nullptr);
		vkFreeMemory(mp_renderer->getDevice(), m_ubo_memory[i], nullptr);
	}
}

void GameObject::Destroy(const int32_t frame)
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].DestroyBuffers(frame);
		if (m_meshes[i].IsCleaned())
		{
			m_meshes.erase(m_meshes.begin() + i);
		}
	}

	vkDestroyBuffer(mp_renderer->getDevice(), m_ubo[frame], nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_ubo_memory[frame], nullptr);
}

void GameObject::registerDrawCmd(VkCommandBuffer& command_buffer, const int32_t frame)
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].draw(command_buffer, frame);
	}
}

void GameObject::bindMatToMesh(const size_t& index, std::shared_ptr<Material> p_material)
{
	for (size_t i = 0; i < m_ubo.size(); i++)
	{
		m_meshes[index].bindMaterial(p_material, m_ubo[i], mp_renderer);
	}
}

void GameObject::LoadMesh(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	Mesh mesh(vertices, indices, mp_renderer);
	m_meshes.push_back(std::move(mesh));

	m_meshes[m_meshes.size() - 1].CreateBuffers(mp_renderer);
}

void GameObject::loadMesh(const std::string& mesh_path)
{
	Mesh mesh(mesh_path, mp_renderer);
	m_meshes.push_back(std::move(mesh));

	m_meshes[m_meshes.size() - 1].CreateBuffers(mp_renderer);
}

void GameObject::UpdateMesh(const size_t& index, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_meshes[index].SetVertices(vertices);
	m_meshes[index].SetIndices(indices);
	m_meshes[index].CreateBuffers(mp_renderer);
}

std::vector<Vertex> GameObject::GetVertices(const size_t& index)
{
	return m_meshes[index].get_vertices();
}

std::vector<uint32_t> GameObject::GetIndices(const size_t& index)
{
	return m_meshes[index].get_indices();
}

Mesh& GameObject::getMesh(const size_t& index)
{
	return m_meshes[index];
}

void GameObject::setMesh(const size_t& index, std::vector<Vertex> vertices)
{
	m_meshes[index].SetVertices(vertices);
	m_meshes[index].CreateBuffers(mp_renderer);
}

void GameObject::setPosition(const glm::vec3& pos)
{
	m_position = pos;
}

const glm::vec3& GameObject::getPosition()
{
	return m_position;
}

void GameObject::setRotation(const glm::vec3& rot)
{
	m_rotation = rot;
}

const glm::vec3& GameObject::getRotation()
{
	return m_rotation;
}

VkBuffer& GameObject::GetUBO(const int32_t frame)
{
	return m_ubo[frame];
}

VkDeviceMemory& GameObject::GetUBOMemory(const int32_t frame)
{
	return m_ubo_memory[frame];
}

const size_t& GameObject::getMeshesCount()
{
	return m_meshes.size();
}

const bool GameObject::Deleted()
{
	return m_meshes.size() == 0;
}

