#ifndef _SCENE_H
#define _SCENE_H

#include <vector>
#include <array>

#include "GameObject.h"
#include "../cameras/Controller.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
public:
	Scene();
	~Scene();

	R3DResult addGameObject(std::shared_ptr<GameObject> gameobject);
	R3DResult removeGameObject(std::shared_ptr<GameObject> gameobject);

	void render(VkCommandBuffer& command_buffer, const int i);
	void Clean();

	void UpdateUBO(std::shared_ptr<Camera> p_camera, std::shared_ptr<Renderer> p_renderer);

	void Update();
	const bool& isUpdate(const int i);
	const bool IsUpdated();
	std::vector<std::shared_ptr<GameObject>>& getObjects();

private:
	std::bitset<3> m_changed;
	std::vector<std::shared_ptr<GameObject>> vp_objects;
	std::vector<std::shared_ptr<GameObject>> vp_delete_queue;
};

#endif //!_SCENE_H