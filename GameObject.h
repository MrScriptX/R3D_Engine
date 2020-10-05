#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>

#include "Renderer.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject(std::shared_ptr<Renderer> p_renderer);
	~GameObject();

	void registerDrawCmd(VkCommandBuffer& command_buffer, Pipeline& pipeline);
	void bindMatToMesh(const size_t& index, std::shared_ptr<Material> p_material);

	void loadMesh(const std::string& mesh_path, std::shared_ptr<Renderer> p_renderer);
	Mesh& getMesh(const size_t& index);

	VkBuffer& getUBO();
	VkDeviceMemory& getUBOMemory();
	
private:
	std::vector<Mesh> m_meshes;

	VkBuffer m_ubo;
	VkDeviceMemory m_ubo_memory;

	std::shared_ptr<Renderer> mp_renderer;
};

#endif