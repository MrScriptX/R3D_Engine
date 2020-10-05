#ifndef _ENGINE_H
#define _ENGINE_H

#include "GameObject.h"
#include "Scene.h"
#include "Renderer.h"

class Engine
{
public:
	Engine();
	~Engine();

	void run();

	void registerGameObject();

private:
	std::shared_ptr<Renderer> mp_renderer;
	std::shared_ptr<Scene> mp_scene;
};

#endif // !_ENGINE_H