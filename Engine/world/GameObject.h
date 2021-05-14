#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <vector>
#include <utility>

#include "Mesh.h"
#include "../errors.h"

class GameObject
{
public:
	GameObject(std::shared_ptr<Renderer> p_renderer);
	~GameObject();

	void Destroy(const int32_t frame);

	void registerDrawCmd(VkCommandBuffer& command_buffer, const int32_t frame);
	void bindMatToMesh(const size_t& index, std::shared_ptr<Material> p_material);

	// MESH
	void LoadMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	void loadMesh(const std::string& mesh_path);
	
	void UpdateMesh(const size_t& index, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

	std::vector<Vertex> GetVertices(const size_t& index);
	std::vector<uint32_t> GetIndices(const size_t& index);

	const std::unique_ptr<Mesh>& getMesh(const size_t& index);
	void setMesh(const size_t& index, std::vector<Vertex> vertices);

	void setPosition(const glm::vec3& pos);
	const glm::vec3& getPosition();

	void setRotation(const glm::vec3& rot);
	const glm::vec3& getRotation();

	VkBuffer& GetUBO(const int32_t frame);
	VkDeviceMemory& GetUBOMemory(const int32_t frame);

	const size_t& getMeshesCount();
	const bool Deleted();
	
private:
	std::vector<std::unique_ptr<Mesh>> m_meshes;

	std::array<VkBuffer, MAX_FRAMES_IN_FLIGHT> m_ubo;
	std::array<VkDeviceMemory, MAX_FRAMES_IN_FLIGHT> m_ubo_memory;

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	std::shared_ptr<Renderer> mp_renderer;
};

#endif