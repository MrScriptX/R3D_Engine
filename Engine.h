#ifndef _ENGINE_H
#define _ENGINE_H

#include "GameObject.h"
#include "Scene.h"
#include "Window.h"

class Engine
{
public:
	Engine();
	~Engine();

	void run();

	void registerGameObject(std::shared_ptr<GameObject> gameobject);

private:
	void input();
	void update();


	std::unique_ptr<Window> mp_window;
	std::shared_ptr<Renderer> mp_renderer;
	std::shared_ptr<Scene> mp_scene;

	std::shared_ptr<Player> mp_player;
	std::shared_ptr<Config> mp_config;

	Pipeline base_pipeline;
	std::chrono::steady_clock::time_point m_last_time;
};

#endif // !_ENGINE_H