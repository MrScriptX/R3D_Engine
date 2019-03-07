#ifndef _TERRAIN_GENERATOR_H
#define _TERRAIN_GENERATOR_H

#include <memory>

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	uint32_t heightGeneration(uint32_t x, uint32_t z);
private:
	
};

#endif // !_TERRAIN_GENERATOR_H