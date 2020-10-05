#include "Scene.h"

Scene::Scene()
{
	m_changed = false;
}

Scene::~Scene()
{
}

void Scene::addGameObject(std::shared_ptr<GameObject> gameobject)
{
	vp_objects.push_back(gameobject);

	m_changed = true;
}

void Scene::render(Pipeline& pipeline, VkCommandBuffer& command_buffer)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects[i]->registerDrawCmd(command_buffer, pipeline);
	}

	m_changed = false;
}

void Scene::updateUBO(std::shared_ptr<Player> p_player, std::shared_ptr<Renderer> p_renderer)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		void* data;
		vkMapMemory(p_renderer->getDevice(), vp_objects[i]->getUBOMemory(), 0, sizeof(p_player->getUBO()), 0, &data);
		memcpy(data, &p_player->getUBO(), sizeof(p_player->getUBO()));
		vkUnmapMemory(p_renderer->getDevice(), vp_objects[i]->getUBOMemory());
	}
}

const bool& Scene::isUpdate()
{
	return m_changed;
}

std::vector<std::shared_ptr<GameObject>>& Scene::getObjects()
{
	return vp_objects;
}
