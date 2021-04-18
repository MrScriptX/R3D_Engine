#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <array>

#include "GameObject.h"
#include "Player.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
public:
	Scene();
	~Scene();

	void addGameObject(std::shared_ptr<GameObject> gameobject);
	void render(Pipeline& pipeline, VkCommandBuffer& command_buffer, const int i);
	void updateUBO(std::shared_ptr<Camera> p_camera, std::shared_ptr<Renderer> p_renderer);

	const bool& isUpdate(const int i);
	std::vector<std::shared_ptr<GameObject>>& getObjects();

private:
	std::array<bool, 2> m_changed;
	std::vector<std::shared_ptr<GameObject>> vp_objects;
};

#endif //!_SCENE_H