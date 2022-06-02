#ifndef R3DENGINE_INCLUDES_ENGINE_H_
#define R3DENGINE_INCLUDES_ENGINE_H_

#include "Window.h"

#include "world/GameObject.h"
#include "world/LightObject.h"
#include "world/Scene.h"

#include "graphics/Geometry.h"

#include "ui/ConsoleUI.h"

class Engine
{
  public:
	Engine(uint32_t width, uint32_t height);
	~Engine();

	void update();
	void draw();

	void setScene(std::shared_ptr<Scene> p_scene);
	void registerGameObject(std::shared_ptr<GameObject> gameobject);

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
	const std::shared_ptr<Camera> GetMainCamera();

	// VIEWING
	void SetWireframeMode();
	void SetPointMode();
	void SetFillMode();
	void SetColorMode(const ColorMode color_map);

	const bool& shouldClose();

  private:
	std::unique_ptr<Window> mp_window;
	std::shared_ptr<Renderer> mp_renderer;
	std::shared_ptr<Scene> mp_scene;

	std::shared_ptr<Camera> mp_main_camera;
	std::shared_ptr<Controller> mp_controller;
	std::shared_ptr<Config> mp_config;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_last_time;

	// UI
	std::vector<UI*> m_UIs;
	ConsoleUI m_console;
};

#endif // !R3DENGINE_INCLUDES_ENGINE_H_
