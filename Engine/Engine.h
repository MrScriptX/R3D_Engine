#ifndef _ENGINE_H
#define _ENGINE_H

#include "Window.h"

#include "world/Scene.h"
#include "world/GameObject.h"

#include "graphics/Geometry.h"

class Engine
{
public:
	Engine();
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

	const bool& shouldClose();

private:

	std::unique_ptr<Window> mp_window;
	std::shared_ptr<Renderer> mp_renderer;
	std::shared_ptr<Scene> mp_scene;

	std::shared_ptr<Camera> mp_main_camera;
	std::shared_ptr<Controller> mp_controller;
	std::shared_ptr<Config> mp_config;
	
	std::chrono::steady_clock::time_point m_last_time;
};

#endif // !_ENGINE_H