#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <memory>
#include <chrono>

#include "Window.h"
#include "Renderer.h"
#include "Player.h"
#include "Mesh.h"
#include "Texture.h"


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
	std::unique_ptr<Mesh> room;
	std::unique_ptr<Mesh> gun;
	std::unique_ptr<Texture> room_txt;
	std::unique_ptr<Texture> gun_txt;

	std::chrono::steady_clock::time_point m_last_time = std::chrono::high_resolution_clock::now();
};

#endif//!_APPLICATION_H