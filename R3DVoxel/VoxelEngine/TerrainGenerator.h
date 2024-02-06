#ifndef ENGINE_TERRAIN_GENERATOR_H_
#define ENGINE_TERRAIN_GENERATOR_H_

#include <cmath>

#include "Chunk.h"
#include "../utils/perlin.h"

class TerrainGenerator
{
public:
    TerrainGenerator();
    ~TerrainGenerator();

    std::unique_ptr<Chunk> SetupSphere(const int32_t posx, const int32_t posy, const int32_t posz);
    std::unique_ptr<Chunk> SetupWorld(const int32_t posx, const int32_t posy, const int32_t posz);

private:
    PerlinNoise m_perlin;
};

#endif //!ENGINE_TERRAIN_GENERATOR_H_