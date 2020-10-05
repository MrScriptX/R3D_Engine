#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<Renderer> p_renderer) : mp_renderer(p_renderer)
{
	m_ubo = VK_NULL_HANDLE;
	m_ubo_memory = VK_NULL_HANDLE;

	mp_renderer->createUBO(m_ubo, m_ubo_memory);
}

GameObject::~GameObject()
{
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

void GameObject::loadMesh(const std::string& mesh_path, std::shared_ptr<Renderer> p_renderer)
{
	Mesh mesh(mesh_path);
	mesh.createBuffer(p_renderer);

	m_meshes.push_back(mesh);
}

Mesh& GameObject::getMesh(const size_t& index)
{
	return m_meshes[index];
}

VkBuffer& GameObject::getUBO()
{
	return m_ubo;
}

VkDeviceMemory& GameObject::getUBOMemory()
{
	return m_ubo_memory;
}

