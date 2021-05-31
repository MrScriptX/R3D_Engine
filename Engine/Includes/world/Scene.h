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

	void Render(VkCommandBuffer& command_buffer, VkDescriptorSet& descriptorset, const int32_t frame);
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
};

#endif // !R3DENGINE_SCENE_H_
