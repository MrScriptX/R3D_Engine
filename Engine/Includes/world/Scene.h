#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <array>

#include "GameObject.h"
#include "../camera/Controller.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
public:
	Scene();
	~Scene();

	R3DResult AddGameObject(std::shared_ptr<GameObject> gameobject);
	R3DResult RemoveGameObject(std::shared_ptr<GameObject> gameobject);

	void Render(VkCommandBuffer& command_buffer, const int32_t frame);
	void Clean(const int32_t frame);

	void UpdateUBO(std::shared_ptr<Camera> p_camera, std::shared_ptr<Renderer> p_renderer, const int32_t frame);

	void ToUpdate();
	void Update(const int32_t frame);
	const bool& isUpdate(const int i);
	const bool IsUpdated();
	std::vector<std::shared_ptr<GameObject>>& getObjects();

private:
	std::bitset<3> m_changed;
	std::vector<std::shared_ptr<GameObject>> vp_objects;
	std::vector<std::shared_ptr<GameObject>> vp_delete_queue;

	VkDescriptorSet m_descritorset;
};

#endif //!_SCENE_H