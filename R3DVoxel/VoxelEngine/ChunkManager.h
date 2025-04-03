#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <map>
#include <utility>
#include <cmath>

#include "../utils/thread_pool.hpp"
#include "../WorldMenu.h"
#include "Chunk.h"
#include "TerrainGenerator.h"

struct render_update_t
{
	int32_t created = -1;
	int32_t update_plus = -1;
	int32_t update_min = -1;

	bool updated = false;
};

class ChunkManager
{
public:
	ChunkManager(std::shared_ptr<GameObject> pworld, std::shared_ptr<Material> p_world_mat, const Camera& camera);
	~ChunkManager() = default;

	void CreateWorld();
	bool UpdateWorld(const Camera& camera);

	render_update_t compute_world_update_x(const Camera& camera);
	render_update_t compute_world_update_z(const Camera& camera);

	void update_world_x(int32_t create_x, int32_t update_xplus, int32_t update_xmin) const;
	void update_world_z(int32_t create_z, int32_t update_zplus, int32_t update_zmin) const;

	void copy_to_render();

	void CreateNewChunk(int32_t x, int32_t y, int32_t z);
	void DestroyChunk(const int32_t x, const int32_t y, const int32_t z);

	WorldMenu& GetMenu();

private:
	std::unique_ptr<Chunk> create_chunk(const int32_t& x, const int32_t& y, const int32_t& z) const;

	std::unique_ptr<TerrainGenerator> mp_terrain_generator;

	std::shared_ptr<GameObject> mp_world;
	std::shared_ptr<Material> mp_world_mat;

	std::map<ChunkKey, std::unique_ptr<Chunk>> _setup_list;
	std::map<ChunkKey, std::unique_ptr<Chunk>> _destroy_list;
	std::map<ChunkKey, std::unique_ptr<Chunk>> m_chunk_map;

	uint8_t m_load_radius = 10;
	glm::vec3 m_render_position;

	glm::i32vec3 m_render_max;
	glm::i32vec3 m_render_min;

	WorldMenu m_worldmenu;

	thread_pool_t& _tpool;
};

#endif