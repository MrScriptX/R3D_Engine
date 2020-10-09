#include "GameObject.h"


GameObject::GameObject(std::shared_ptr<Renderer> p_renderer) : mp_renderer(p_renderer)
{
	m_ubo = VK_NULL_HANDLE;
	m_ubo_memory = VK_NULL_HANDLE;

	m_position = { 0, 0, 0 };
	m_rotation = { 0, 0, 0 };

	mp_renderer->createUBO(m_ubo, m_ubo_memory);
}

GameObject::~GameObject()
{
}

void GameObject::destroy()
{
	vkDestroyBuffer(mp_renderer->getDevice(), m_ubo, nullptr);
	vkFreeMemory(mp_renderer->getDevice(), m_ubo_memory, nullptr);
}

void GameObject::registerDrawCmd(VkCommandBuffer& command_buffer, Pipeline& pipeline)
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].draw(command_buffer, pipeline);
	}
}

void GameObject::bindMatToMesh(const size_t& index, std::shared_ptr<Material> p_material)
{
	m_meshes[index].bindMaterial(p_material, m_ubo, mp_renderer);
}

void GameObject::loadMesh(const std::string& mesh_path)
{
	Mesh mesh(mesh_path, mp_renderer);
	mesh.createBuffer(mp_renderer);

	m_meshes.push_back(mesh);
}

Mesh& GameObject::getMesh(const size_t& index)
{
	return m_meshes[index];
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

VkBuffer& GameObject::getUBO()
{
	return m_ubo;
}

VkDeviceMemory& GameObject::getUBOMemory()
{
	return m_ubo_memory;
}

const size_t& GameObject::getMeshesCount()
{
	return m_meshes.size();
}

