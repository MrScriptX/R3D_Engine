#ifndef _SCENE_H
#define _SCENE_H

#include <vector>

#include "GameObject.h"
#include "Player.h"

class Scene
{
public:
	Scene();
	~Scene();

	void addGameObject(std::shared_ptr<GameObject> gameobject);
	void render(Pipeline& pipeline, VkCommandBuffer& command_buffer);
	void updateUBO(std::shared_ptr<Player> p_player, std::shared_ptr<Renderer> p_renderer);

	const bool& isUpdate();
	std::vector<std::shared_ptr<GameObject>>& getObjects();

private:
	bool m_changed;
	std::vector<std::shared_ptr<GameObject>> vp_objects;
};

#endif //!_SCENE_H