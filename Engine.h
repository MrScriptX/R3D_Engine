#ifndef _ENGINE_H
#define _ENGINE_H

#include "Renderer.h"

class Engine
{
public:
	Engine();
	~Engine();

	void run();

private:
	std::shared_ptr<Renderer> mp_renderer;
};

#endif // !_ENGINE_H