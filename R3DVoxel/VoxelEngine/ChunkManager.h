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

	std::optional<render_update_t> compute_world_update_x(const Camera& camera);
	std::optional<render_update_t> compute_world_update_z(const Camera& camera);

	std::map<ChunkKey, Geometry> compute_meshes(std::optional<render_update_t> x, std::optional<render_update_t> z);
	void render_meshes(const std::map<ChunkKey, Geometry>& meshes, std::optional<render_update_t> x, std::optional<render_update_t> z);

	void copy_to_world();
	void copy_to_render();

	WorldMenu& GetMenu();

private:
	render_update_t update_x_forward(const Camera& camera);
	render_update_t update_x_backward(const Camera& camera);
	render_update_t update_z_forward(const Camera& camera);
	render_update_t update_z_backward(const Camera& camera);

	std::unique_ptr<Chunk> create_chunk(const int32_t& x, const int32_t& y, const int32_t& z) const;

	std::unique_ptr<TerrainGenerator> mp_terrain_generator;

	std::shared_ptr<GameObject> mp_world;
	std::shared_ptr<Material> mp_world_mat;


	std::map<ChunkKey, std::unique_ptr<Chunk>> _setup_list;
	std::map<ChunkKey, std::unique_ptr<Chunk>> _destroy_list;
	std::map<ChunkKey, std::unique_ptr<Chunk>> _chunk_map;

	uint8_t m_load_radius = 10;
	glm::vec3 m_render_position;

	glm::i32vec3 m_render_max;
	glm::i32vec3 m_render_min;

	WorldMenu m_worldmenu;

	thread_pool_t& _tpool;
};

#endif