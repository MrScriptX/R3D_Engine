#ifndef R3DENGINE_SCENE_H_
#define R3DENGINE_SCENE_H_

#include <array>
#include <vector>

#include "../camera/Controller.h"
#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Scene
{
  public:
	Scene();
	~Scene();

	R3DResult AddGameObject(std::shared_ptr<GameObject> gameobject);
	R3DResult RemoveGameObject(std::shared_ptr<GameObject> gameobject);
	R3DResult AddLight(std::shared_ptr<LightObject> lightobject);
	R3DResult RemoveLight(std::shared_ptr<LightObject> lightobject);

	void Load(std::shared_ptr<Renderer> p_renderer);
	void Render(VkCommandBuffer& command_buffer, const int32_t frame);
	void Clean(const int32_t frame);
	void CleanRessources(std::shared_ptr<Renderer> p_renderer);

	void UpdateUBO(std::shared_ptr<Camera> p_camera, std::shared_ptr<Renderer> p_renderer, const int32_t frame);
	void UpdateSceneUBO(std::shared_ptr<Renderer> p_renderer);

	void ToUpdate();
	void Update(const int32_t frame);
	const bool& isUpdate(const int i);
	const bool IsUpdated();
	std::vector<std::shared_ptr<GameObject>>& getObjects();

  private:
	R3DResult addDirectionalLight(std::shared_ptr<LightObject> lightobject);
	R3DResult addSpotLight(std::shared_ptr<LightObject> lightobject);
	R3DResult addPointLight(std::shared_ptr<LightObject> lightobject);
	R3DResult removeDirectionalLight(std::shared_ptr<LightObject> lightobject);
	R3DResult removeSpotLight(std::shared_ptr<LightObject> lightobject);
	R3DResult removePointLight(std::shared_ptr<LightObject> lightobject);

	std::bitset<3> m_changed;
	bool m_light_changed;

	// game object
	std::vector<std::shared_ptr<GameObject>> vp_objects;
	std::vector<std::shared_ptr<GameObject>> vp_delete_queue;

	// light
	std::array<std::shared_ptr<LightObject>, MAX_LIGHT> vp_directional_lights;
	std::array<std::shared_ptr<LightObject>, MAX_LIGHT> vp_spot_lights;
	std::array<std::shared_ptr<LightObject>, MAX_LIGHT> vp_point_lights;

	VkBuffer m_light_buffer;
	VkDeviceMemory m_light_mem;
	VkDescriptorSet m_descriptorset;
};

#endif // !R3DENGINE_SCENE_H_
