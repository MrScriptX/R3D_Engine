#pragma once

#include <memory>
#include <chrono>

#include "Window.h"
#include "Renderer.h"
#include "ChunkManager.h"
#include "Player.h"

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

	std::unique_ptr<ChunkManager> m_pChunkManager;
	std::shared_ptr<Player> m_pPlayer;

	Pipeline base_pipeline;
	Camera m_camera;

	std::chrono::steady_clock::time_point m_last_time = std::chrono::high_resolution_clock::now();
};

