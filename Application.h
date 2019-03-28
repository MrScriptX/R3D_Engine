#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <memory>
#include <chrono>

#include "Window.h"
#include "Renderer.h"
#include "Player.h"
#include "Model.h"


class Application
{
public:
	Application();
	~Application();


	void run();


private:

	void update();
	void input();

	std::unique_ptr<Config> m_config;
	std::unique_ptr<Window> m_window;
	std::shared_ptr<Renderer> m_pRenderer;

	std::shared_ptr<Player> m_pPlayer;

	Pipeline base_pipeline;
	Camera m_camera;
	Model model;

	std::chrono::steady_clock::time_point m_last_time = std::chrono::high_resolution_clock::now();
};

#endif//!_APPLICATION_H