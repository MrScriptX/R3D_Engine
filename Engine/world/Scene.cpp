#include "Scene.h"


Scene::Scene()
{
	m_changed.fill(false);
}

Scene::~Scene()
{
}

R3DResult Scene::addGameObject(std::shared_ptr<GameObject> gameobject)
{
	// check if object is not in the scene
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		if (vp_objects[i] == gameobject)
		{
			return R3DResult::R3D_OBJECT_IN_SCENE;
		}
	}

	// add object to scene
	vp_objects.push_back(gameobject);
	m_changed.fill(true);

	return R3DResult::R3D_SUCCESS;
}

R3DResult Scene::removeGameObject(std::shared_ptr<GameObject> gameobject)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		if (vp_objects[i] == gameobject)
		{
			vp_objects.erase(vp_objects.begin() + i);
			m_changed.fill(true);

			return R3DResult::R3D_SUCCESS;
		}
	}

	return R3DResult::R3D_OBJECT_NOT_FOUND;
}

void Scene::render(Pipeline& pipeline, VkCommandBuffer& command_buffer, const int i)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects[i]->registerDrawCmd(command_buffer, pipeline);
	}

	m_changed[i] = false;
}

void Scene::updateUBO(std::shared_ptr<Camera> p_camera, std::shared_ptr<Renderer> p_renderer)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		UniformBufferObject ubo = p_camera->getUBO();
		
		glm::mat4 matrix = glm::mat4(1.0f);

		matrix = glm::translate(matrix, vp_objects[i]->getPosition());

		matrix = glm::rotate(matrix, glm::radians(vp_objects[i]->getRotation().x), { 1, 0, 0 });
		matrix = glm::rotate(matrix, glm::radians(vp_objects[i]->getRotation().y), { 0, 1, 0 });
		matrix = glm::rotate(matrix, glm::radians(vp_objects[i]->getRotation().z), { 0, 0, 1 });

		ubo.model = matrix;

		void* data;
		vkMapMemory(p_renderer->getDevice(), vp_objects[i]->getUBOMemory(), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(p_camera->getUBO()));
		vkUnmapMemory(p_renderer->getDevice(), vp_objects[i]->getUBOMemory());
	}
}

const bool& Scene::isUpdate(const int i)
{
	return m_changed[i];
}

std::vector<std::shared_ptr<GameObject>>& Scene::getObjects()
{
	return vp_objects;
}
