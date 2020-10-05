#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <memory>
#include <chrono>

#include "Window.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Material.h"
#include "Scene.h"


class Application
{
public:
	Application();
	~Application();

	void run();

private:

	void update();
	void input();

	std::shared_ptr<Config> m_config;
	std::unique_ptr<Window> m_window;
	std::shared_ptr<Renderer> m_pRenderer;

	std::shared_ptr<Player> m_pPlayer;

	Pipeline base_pipeline;
	Camera m_camera;


	std::shared_ptr<Material> gun_texture;
	std::shared_ptr<GameObject> gun;

	std::shared_ptr<Material> room_texture;
	std::shared_ptr<GameObject> room;

	Scene scene;

	std::chrono::steady_clock::time_point m_last_time = std::chrono::high_resolution_clock::now();
};

#endif//!_APPLICATION_H