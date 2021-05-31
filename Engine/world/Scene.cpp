#include "../Includes/world/Scene.h"

Scene::Scene()
{
	m_changed.reset();
}

Scene::~Scene()
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects.erase(vp_objects.begin() + i);
		--i;
	}

	for (size_t i = 0; i < vp_delete_queue.size(); i++)
	{
		vp_delete_queue.erase(vp_objects.begin() + i);
		--i;
	}
}

R3DResult Scene::AddGameObject(std::shared_ptr<GameObject> gameobject)
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
	m_changed.set();

	return R3DResult::R3D_SUCCESS;
}

R3DResult Scene::RemoveGameObject(std::shared_ptr<GameObject> gameobject)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		if (vp_objects[i] == gameobject)
		{
			vp_delete_queue.push_back(std::move(vp_objects[i]));
			vp_objects.erase(vp_objects.begin() + i);

			m_changed.set();

			return R3DResult::R3D_SUCCESS;
		}
	}

	return R3DResult::R3D_OBJECT_NOT_FOUND;
}

void Scene::Render(VkCommandBuffer& command_buffer, VkDescriptorSet& descriptorset, const int32_t frame)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects[i]->RegisterDrawCmd(command_buffer, descriptorset, frame);
	}

	m_changed.set(frame, false);
}

void Scene::Clean(const int32_t frame)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects[i]->Clean(frame);
	}

	for (size_t i = 0; i < vp_delete_queue.size(); i++)
	{
		vp_delete_queue[i]->Destroy(frame);
		if (vp_delete_queue[i]->Deleted())
		{
			vp_delete_queue.erase(vp_delete_queue.begin() + i);
			--i;
		}
	}
}

void Scene::UpdateUBO(std::shared_ptr<Camera> p_camera, std::shared_ptr<Renderer> p_renderer, const int32_t frame)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		UniformBufferObject ubo = p_camera->GetUBO(frame);

		glm::mat4 matrix = glm::mat4(1.0f);

		matrix = glm::translate(matrix, vp_objects[i]->getPosition());

		matrix = glm::rotate(matrix, glm::radians(vp_objects[i]->getRotation().x), { 1, 0, 0 });
		matrix = glm::rotate(matrix, glm::radians(vp_objects[i]->getRotation().y), { 0, 1, 0 });
		matrix = glm::rotate(matrix, glm::radians(vp_objects[i]->getRotation().z), { 0, 0, 1 });

		ubo.model = matrix;

		void* data;
		vkMapMemory(p_renderer->getDevice(), vp_objects[i]->GetUBOMemory(frame), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(p_camera->GetUBO(frame)));
		vkUnmapMemory(p_renderer->getDevice(), vp_objects[i]->GetUBOMemory(frame));
	}
}

void Scene::ToUpdate()
{
	m_changed.set();
}

void Scene::Update(const int32_t frame)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects[i]->Update(frame);
	}
}

const bool& Scene::isUpdate(const int i)
{
	return m_changed[i];
}

const bool Scene::IsUpdated()
{
	return m_changed == false;
}

std::vector<std::shared_ptr<GameObject>>& Scene::getObjects()
{
	return vp_objects;
}
