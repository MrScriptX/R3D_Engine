#include "../Includes/world/GameObject.h"

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
	m_meshes.clear();

	for (size_t i = 0; i < m_ubo.size(); i++)
	{
		vkDestroyBuffer(mp_renderer->getDevice(), m_ubo[i], nullptr);
		vkFreeMemory(mp_renderer->getDevice(), m_ubo_memory[i], nullptr);
	}
}

void GameObject::Update(const int32_t frame)
{
	for (size_t i = 0; i < m_mesh_to_update.size(); i++)
	{
		m_meshes[m_mesh_to_update[i]]->UpdateBuffers(frame);

		if (m_meshes[m_mesh_to_update[i]]->IsUpdated())
		{
			m_mesh_to_update.erase(m_mesh_to_update.begin() + i);
			--i;
		}
	}
}

void GameObject::Clean(const size_t frame)
{
	for (size_t i = 0; i < m_mesh_to_delete.size(); i++)
	{
		m_mesh_to_delete[i]->DestroyBuffers(frame);
		if (m_mesh_to_delete[i]->IsCleaned())
		{
			m_mesh_to_delete.erase(m_mesh_to_delete.begin() + i);
			--i;
		}
	}
}

void GameObject::Destroy(const int32_t frame)
{
	for (auto it = m_meshes.begin(); it != m_meshes.end();)
	{
		it->second->DestroyBuffers(frame);
		if (it->second->IsCleaned())
		{
			it = m_meshes.erase(it);
		}
		else
		{
			++it;
		}
	}

	vkDestroyBuffer(mp_renderer->getDevice(), m_ubo[frame], nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_ubo_memory[frame], nullptr);

	m_ubo[frame] = VK_NULL_HANDLE;
	m_ubo_memory[frame] = VK_NULL_HANDLE;
}

void GameObject::RegisterDrawCmd(VkCommandBuffer& command_buffer, VkDescriptorSet& descriptorset, const int32_t frame)
{
	for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		it->second->draw(command_buffer, descriptorset, frame);
	}
}

void GameObject::bindMatToMesh(const size_t& index, std::shared_ptr<Material> p_material)
{
	for (size_t i = 0; i < m_ubo.size(); i++)
	{
		m_meshes[index]->bindMaterial(p_material, m_ubo[i], mp_renderer);
	}
}

int32_t GameObject::LoadMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	int32_t i = m_meshes.rbegin() != m_meshes.rend() ? m_meshes.rbegin()->first + 1 : 0;
	m_meshes.insert({ i, std::make_unique<Mesh>(vertices, indices, mp_renderer) });
	m_meshes.at(i)->CreateBuffers(mp_renderer);

	return i;
}

int32_t GameObject::LoadMesh(const std::string& mesh_path)
{
	int32_t i = m_meshes.rbegin() != m_meshes.rend() ? m_meshes.rbegin()->first + 1 : 0;
	m_meshes.insert({ i, std::make_unique<Mesh>(mesh_path, mp_renderer) });
	m_meshes[i]->CreateBuffers(mp_renderer);

	return i;
}

void GameObject::UpdateMesh(const size_t& index, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	m_meshes[index]->SetVertices(vertices);
	m_meshes[index]->SetIndices(indices);

	m_mesh_to_update.push_back(index);
}

void GameObject::RemoveMesh(const size_t index)
{
	m_mesh_to_delete.push_back(std::move(m_meshes[index]));
	m_meshes.erase(index);
}

std::vector<Vertex> GameObject::GetVertices(const size_t& index)
{
	return m_meshes[index]->get_vertices();
}

std::vector<uint32_t> GameObject::GetIndices(const size_t& index)
{
	return m_meshes[index]->get_indices();
}

const std::unique_ptr<Mesh>& GameObject::getMesh(const size_t& index)
{
	return m_meshes[index];
}

void GameObject::setMesh(const size_t& index, std::vector<Vertex> vertices)
{
	m_meshes[index]->SetVertices(vertices);
	m_meshes[index]->CreateBuffers(mp_renderer);
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
