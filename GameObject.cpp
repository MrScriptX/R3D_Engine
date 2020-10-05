#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<Renderer> p_renderer)
{
	m_ubo = VK_NULL_HANDLE;
	m_ubo_memory = VK_NULL_HANDLE;

	p_renderer->createUBO(m_ubo, m_ubo_memory);
}

GameObject::~GameObject()
{
}

void GameObject::loadMesh(const std::string& mesh_path, std::shared_ptr<Renderer> p_renderer)
{
	Mesh mesh(mesh_path);
	mesh.createBuffer(p_renderer);

	m_meshes.push_back(mesh);
}

std::vector<Mesh>& GameObject::getMeshes()
{
	return m_meshes;
}

VkBuffer& GameObject::getUBO()
{
	return m_ubo;
}

VkDeviceMemory& GameObject::getUBOMemory()
{
	return m_ubo_memory;
}

