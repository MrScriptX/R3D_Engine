#ifndef _ENGINE_H
#define _ENGINE_H

#include "GameObject.h"
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

	std::vector<std::shared_ptr<GameObject>> vp_objects;
};

#endif // !_ENGINE_H