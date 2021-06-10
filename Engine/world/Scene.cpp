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

R3DResult Scene::AddLight(std::shared_ptr<LightObject> lightobject)
{
	switch (lightobject->GetType())
	{
	case TLIGHT::DIRECTIONAL:
		return addDirectionalLight(lightobject);
	case TLIGHT::SPOT:
		return addSpotLight(lightobject);
	case TLIGHT::POINT:
		return addPointLight(lightobject);
	default:
		return R3DResult::R3D_OBJECT_NOT_ENOUGH_MEMORY; // change error
	}
}

R3DResult Scene::RemoveLight(std::shared_ptr<LightObject> lightobject)
{
	switch (lightobject->GetType())
	{
	case TLIGHT::DIRECTIONAL:
		return removeDirectionalLight(lightobject);
	case TLIGHT::SPOT:
		return removeSpotLight(lightobject);
	case TLIGHT::POINT:
		return removePointLight(lightobject);
	default:
		return R3DResult::R3D_OBJECT_NOT_FOUND; // change error
		break;
	}
}

void Scene::Load(std::shared_ptr<Renderer> p_renderer)
{
	p_renderer->allocateDescriptorSetLight(m_descriptorset);
	p_renderer->CreateUniformBuffer(m_light_buffer, m_light_mem, sizeof(SceneUBO));
	p_renderer->updateDescriptorSet(m_light_buffer, m_descriptorset, sizeof(SceneUBO));
}

void Scene::Render(VkCommandBuffer& command_buffer, const int32_t frame)
{
	for (size_t i = 0; i < vp_objects.size(); i++)
	{
		vp_objects[i]->RegisterDrawCmd(command_buffer, m_descriptorset, frame);
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

void Scene::CleanRessources(std::shared_ptr<Renderer> p_renderer)
{
	vkDestroyBuffer(p_renderer->GetDevice(), m_light_buffer, nullptr);
	vkFreeMemory(p_renderer->GetDevice(), m_light_mem, nullptr);

	m_light_buffer = VK_NULL_HANDLE;
	m_light_mem = VK_NULL_HANDLE;
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
		vkMapMemory(p_renderer->GetDevice(), vp_objects[i]->GetUBOMemory(frame), 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(p_camera->GetUBO(frame)));
		vkUnmapMemory(p_renderer->GetDevice(), vp_objects[i]->GetUBOMemory(frame));
	}
}

void Scene::UpdateSceneUBO(std::shared_ptr<Renderer> p_renderer)
{
	SceneUBO ubo;
	for (size_t i = 0; i < vp_directional_lights.max_size(); i++)
	{
		if (vp_directional_lights[i] != nullptr)
		{
			ubo.directionals[i] = vp_directional_lights[i]->GetProperties();
			++ubo.nb_directional;
		}
	}

	for (size_t i = 0; i < vp_spot_lights.max_size(); i++)
	{
		if (vp_spot_lights[i] != nullptr)
		{
			ubo.spots[i] = vp_spot_lights[i]->GetProperties();
			++ubo.nb_spotlight;
		}
	}

	for (size_t i = 0; i < vp_point_lights.max_size(); i++)
	{
		if (vp_point_lights[i] != nullptr)
		{
			ubo.points[i] = vp_point_lights[i]->GetProperties();
			++ubo.nb_pointlight;
		}
	}

	void* data;
	vkMapMemory(p_renderer->GetDevice(), m_light_mem, 0, sizeof(SceneUBO), 0, &data);
	memcpy(data, &ubo, sizeof(SceneUBO));
	vkUnmapMemory(p_renderer->GetDevice(), m_light_mem);

	m_light_changed = false;
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

R3DResult Scene::addDirectionalLight(std::shared_ptr<LightObject> lightobject)
{
	int32_t empty_index = -1;
	for (size_t i = 0; i < vp_directional_lights.max_size(); i++)
	{
		if (vp_directional_lights[i] == lightobject)
		{
			return R3DResult::R3D_OBJECT_IN_SCENE;
		}
		else if (vp_directional_lights[i] == nullptr && empty_index == -1)
		{
			empty_index = i;
		}
	}

	if (empty_index == -1)
		return R3DResult::R3D_OBJECT_NOT_ENOUGH_MEMORY;
	else
		vp_directional_lights[empty_index] = lightobject;

	m_light_changed = true;

	return R3DResult::R3D_SUCCESS;
}

R3DResult Scene::addSpotLight(std::shared_ptr<LightObject> lightobject)
{
	int32_t empty_index = -1;
	for (size_t i = 0; i < vp_spot_lights.max_size(); i++)
	{
		if (vp_spot_lights[i] == lightobject)
		{
			return R3DResult::R3D_OBJECT_IN_SCENE;
		}
		else if (vp_spot_lights[i] == nullptr && empty_index == -1)
		{
			empty_index = i;
		}
	}

	if (empty_index == -1)
		return R3DResult::R3D_OBJECT_NOT_ENOUGH_MEMORY;
	else
		vp_spot_lights[empty_index] = lightobject;

	m_light_changed = true;

	return R3DResult::R3D_SUCCESS;
}

R3DResult Scene::addPointLight(std::shared_ptr<LightObject> lightobject)
{
	int32_t empty_index = -1;
	for (size_t i = 0; i < vp_point_lights.max_size(); i++)
	{
		if (vp_point_lights[i] == lightobject)
		{
			return R3DResult::R3D_OBJECT_IN_SCENE;
		}
		else if (vp_point_lights[i] == nullptr && empty_index == -1)
		{
			empty_index = i;
		}
	}

	if (empty_index == -1)
		return R3DResult::R3D_OBJECT_NOT_ENOUGH_MEMORY;
	else
		vp_point_lights[empty_index] = lightobject;

	m_light_changed = true;

	return R3DResult::R3D_SUCCESS;
}

R3DResult Scene::removeDirectionalLight(std::shared_ptr<LightObject> lightobject)
{
	for (size_t i = 0; i < vp_directional_lights.max_size(); i++)
	{
		if (vp_directional_lights[i] == lightobject)
		{
			vp_directional_lights[i] = nullptr;
			m_light_changed = true;

			return R3DResult::R3D_SUCCESS;
		}
	}

	return R3DResult::R3D_OBJECT_NOT_FOUND;
}

R3DResult Scene::removeSpotLight(std::shared_ptr<LightObject> lightobject)
{
	for (size_t i = 0; i < vp_spot_lights.max_size(); i++)
	{
		if (vp_spot_lights[i] == lightobject)
		{
			vp_spot_lights[i] = nullptr;
			m_light_changed = true;

			return R3DResult::R3D_SUCCESS;
		}
	}

	return R3DResult::R3D_OBJECT_NOT_FOUND;
}

R3DResult Scene::removePointLight(std::shared_ptr<LightObject> lightobject)
{
	for (size_t i = 0; i < vp_point_lights.max_size(); i++)
	{
		if (vp_point_lights[i] == lightobject)
		{
			vp_point_lights[i] = nullptr;
			m_light_changed = true;

			return R3DResult::R3D_SUCCESS;
		}
	}

	return R3DResult::R3D_OBJECT_NOT_FOUND;
}
