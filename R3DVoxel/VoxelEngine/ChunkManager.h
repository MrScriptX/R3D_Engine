#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <map>
#include <utility>
#include <cmath>

#include "../WorldMenu.h"
#include "Chunk.h"
#include "TerrainGenerator.h"

class ChunkManager
{
public:
	ChunkManager(std::shared_ptr<GameObject> pworld, std::shared_ptr<Material> p_world_mat, std::shared_ptr<Camera> p_camera);
	~ChunkManager() = default;

	void CreateWorld();
	void UpdateWorld(std::shared_ptr<Scene> p_scene, std::shared_ptr<Camera> p_camera);

	void CreateNewChunk(int32_t x, int32_t y, int32_t z);
	void DestroyChunk(const int32_t x, const int32_t y, const int32_t z);

	WorldMenu& GetMenu();

private:
	std::unique_ptr<Chunk> create_chunk(const int32_t& x, const int32_t& y, const int32_t& z) const;

	std::unique_ptr<TerrainGenerator> mp_terrain_generator;

	std::shared_ptr<GameObject> mp_world;
	std::shared_ptr<Material> mp_world_mat;

	std::map<ChunkKey, std::unique_ptr<Chunk>> m_setup_list;
	std::map<ChunkKey, std::unique_ptr<Chunk>> m_chunk_map;
	uint8_t m_load_radius = 10;
	glm::vec3 m_render_position;

	glm::i32vec3 m_render_max;
	glm::i32vec3 m_render_min;

	WorldMenu m_worldmenu;
};

#endif