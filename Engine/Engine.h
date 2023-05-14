#ifndef R3DENGINE_INCLUDES_ENGINE_H_
#define R3DENGINE_INCLUDES_ENGINE_H_

#include <unordered_map>

#include "Window.h"

#include "world/GameObject.h"
#include "world/LightObject.h"
#include "world/Scene.h"

#include "graphics/Geometry.h"

#include "ui/ConsoleUI.h"
#include "ui/Watcher.h"

#include "renderer/vpipeline.h"
#include "renderer/pipeline.h"

class Engine
{
  public:
	Engine(uint32_t width, uint32_t height);
	~Engine();

	void update();
	void draw();

	void setScene(std::shared_ptr<Scene> p_scene);
	void registerGameObject(std::shared_ptr<GameObject> gameobject);

	uint16_t create_pipeline(const vred::renderer::shader_stages& shaders);

	// CREATE MATERIAL
	const std::shared_ptr<Material> CreateMaterial(const TSHADER shader);
	const std::shared_ptr<Material> CreateMaterial(const TSHADER shader, const std::string& texture_file);

	// CREATE GAMEOBJECT
	const std::shared_ptr<GameObject> CreateGameObject();
	const std::shared_ptr<GameObject> CreateGameObject(const std::string& object_file);

	const std::shared_ptr<GameObject> CreateCube(const glm::vec3& position, const float& size, const glm::vec3& vcolor);

	// CONTROLLER
	void BindKeyToFunc(const int& key, std::function<void()>& func, const ActionType& type = ActionType::R3D_PRESS);

	// CAMERA
	std::shared_ptr<Camera> GetMainCamera() const;

	// VIEWING
	void SetWireframeMode();
	void SetPointMode();
	void SetFillMode();
	void SetColorMode(const ColorMode color_map);

	void RenderUI(UI& ui);
	void RemoveUI(const UI& ui);

	bool shouldClose() const;

  private:
	std::unique_ptr<Window> mp_window;
	std::shared_ptr<Renderer> mp_renderer;
	std::shared_ptr<Scene> mp_scene;

	std::shared_ptr<Camera> mp_main_camera;
	std::shared_ptr<Controller> mp_controller;
	std::shared_ptr<Config> mp_config;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_last_time;

	std::unordered_map<uint16_t, vred::renderer::ipipeline> m_pipelines;

	// UI
	std::vector<UI*> m_UIs;
};

#endif // !R3DENGINE_INCLUDES_ENGINE_H_
