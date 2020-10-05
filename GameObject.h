#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>

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

	void setPosition(const glm::vec3& pos);
	const glm::vec3& getPosition();

	void setRotation(const glm::vec3& rot);
	const glm::vec3& getRotation();

	VkBuffer& getUBO();
	VkDeviceMemory& getUBOMemory();
	
private:
	std::vector<Mesh> m_meshes;

	VkBuffer m_ubo;
	VkDeviceMemory m_ubo_memory;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	std::shared_ptr<Renderer> mp_renderer;
};

#endif