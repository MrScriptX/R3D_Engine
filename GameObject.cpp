#include "GameObject.h"

GameObject::GameObject()
{
	m_ubo = VK_NULL_HANDLE;
	m_ubo_memory = VK_NULL_HANDLE;
}

GameObject::~GameObject()
{
}

void GameObject::loadMesh(const std::string& mesh_path)
{
	Mesh mesh(mesh_path);
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

