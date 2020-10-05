#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>

#include "Renderer.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void loadMesh(const std::string& mesh_path, std::shared_ptr<Renderer> p_renderer);
	std::vector<Mesh>& getMeshes();

	VkBuffer& getUBO();
	VkDeviceMemory& getUBOMemory();
	
private:
	std::vector<Mesh> m_meshes;

	VkBuffer m_ubo;
	VkDeviceMemory m_ubo_memory;
};

#endif