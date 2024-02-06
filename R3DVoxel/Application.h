#ifndef APPLICATION_H
#define APPLICATION_H

#include <Engine.h>

#include "VoxelEngine/ChunkManager.h"

class Application
{
public:
	Application();

	void Start();

private:
	std::unique_ptr<Engine> mp_engine;

};

#endif // !APPLICATION_H