#include "ChunkManager.h"

#include <utility>
#include <barrier>

ChunkManager::ChunkManager(std::shared_ptr<GameObject> pworld, std::shared_ptr<Material> p_world_mat, const Camera& camera) :
	mp_world(std::move(pworld)), mp_world_mat(std::move(p_world_mat)), m_worldmenu(m_load_radius), _tpool(thread_pool_t::instance())
{
	mp_terrain_generator = std::make_unique<TerrainGenerator>();
	m_render_position = camera.GetPosition();
	m_render_max = { m_render_position.x + m_load_radius, m_render_position.y + m_load_radius, m_render_position.z + m_load_radius };
	m_render_min = { m_render_position.x - m_load_radius, m_render_position.y - m_load_radius, m_render_position.z - m_load_radius };
}

void ChunkManager::CreateWorld()
{
	// preallocate memory for chunks
	std::map<ChunkKey, std::unique_ptr<Chunk>> chunks;
	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
			{
				const ChunkKey key = { .x = x, .y = y, .z = z};
				chunks.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, nullptr));
			}
		}
	}

	// compute chunks in parallel
	const int64_t num_threads = (m_render_max.x - m_render_min.x) + 2;
	std::barrier sync(num_threads, [this, &chunks]() noexcept { m_chunk_map = std::move(chunks); });

	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		auto task = [x, &chunks, &sync, this]()
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
				{
					chunks.at({ x, y, z }) = create_chunk(x, y, z);
				}
			}
			sync.arrive_and_drop();
		};

		_tpool.enqueue(task);
	}

	sync.arrive_and_wait(); // wait for all chunks to be created

	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
			{
				m_chunk_map.at({ x, y, z })->BuildChunk(m_chunk_map, mp_world, mp_world_mat);
			}
		}
	}
}

bool ChunkManager::UpdateWorld(const Camera& camera)
{
	bool scene_x_need_update = false;

	int32_t create_x = -1;
	int32_t update_xplus = -1;
	int32_t update_xmin = -1;
	if (camera.GetPosition().x > m_render_position.x + Voxel::CHUNK_SIZE)
	{
		m_render_position.x = camera.GetPosition().x;

		m_render_max.x = m_render_max.x + 1;

		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				CreateNewChunk(m_render_max.x, y, z);
				DestroyChunk(m_render_min.x, y, z);
			}
		}

		m_render_min.x = m_render_min.x + 1;

		create_x = m_render_max.x;
		update_xplus = m_render_max.x - 1;
		update_xmin = m_render_min.x;
		scene_x_need_update = true;
	}
	else if (camera.GetPosition().x < m_render_position.x - Voxel::CHUNK_SIZE)
	{
		m_render_position.x = camera.GetPosition().x;

		m_render_min.x = m_render_min.x - 1;

		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				CreateNewChunk(m_render_min.x, y, z);
				DestroyChunk(m_render_max.x, y, z);
			}
		}

		m_render_max.x = m_render_max.x - 1;

		create_x = m_render_min.x;
		update_xplus = m_render_min.x + 1;
		update_xmin = m_render_max.x;
		scene_x_need_update = true;
	}

	bool scene_z_need_update = false;

	int32_t create_z = -1;
	int32_t update_zplus = -1;
	int32_t update_zmin = -1;
	if (camera.GetPosition().z > m_render_position.z + Voxel::CHUNK_SIZE)
	{
		m_render_position.z = camera.GetPosition().z;

		m_render_max.z = m_render_max.z + 1;

		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				CreateNewChunk(x, y, m_render_max.z);
				DestroyChunk(x, y, m_render_min.z);
			}
		}

		m_render_min.z = m_render_min.z + 1;

		create_z = m_render_max.z;
		update_zplus = m_render_max.z - 1;
		update_zmin = m_render_min.z;
		scene_z_need_update = true;
	}
	else if (camera.GetPosition().z < m_render_position.z - Voxel::CHUNK_SIZE)
	{
		m_render_position.z = camera.GetPosition().z;

		m_render_min.z = m_render_min.z - 1;

		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				CreateNewChunk(x, y, m_render_min.z);
				DestroyChunk(x, y, m_render_max.z);
			}	
		}

		m_render_max.z = m_render_max.z - 1;

		create_z = m_render_min.z;
		update_zplus = m_render_min.z + 1;
		update_zmin = m_render_max.z;
		scene_z_need_update = true;
	}

	if (scene_x_need_update)
	{
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				m_chunk_map.at({ create_x, y, z })->BuildChunk(m_chunk_map, mp_world, mp_world_mat);
				m_chunk_map.at({ update_xplus, y, z })->UpdateChunk(m_chunk_map, *mp_world);
				m_chunk_map.at({ update_xmin, y, z })->UpdateChunk(m_chunk_map, *mp_world);
			}
		}
	}
	
	if (scene_z_need_update)
	{
		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				m_chunk_map.at({ x, y, create_z })->BuildChunk(m_chunk_map, mp_world, mp_world_mat);
				m_chunk_map.at({ x, y, update_zplus })->UpdateChunk(m_chunk_map, *mp_world);
				m_chunk_map.at({ x, y, update_zmin })->UpdateChunk(m_chunk_map, *mp_world);
			}
		}
	}

	Watcher::WatchPosition("render position", m_render_position);
	Watcher::WatchPosition("render max", m_render_max);
	Watcher::WatchPosition("render min", m_render_min);

	return scene_x_need_update || scene_z_need_update;
}

render_update_t ChunkManager::compute_world_update_x(const Camera& camera)
{
	render_update_t render_update;

	if (camera.GetPosition().x > m_render_position.x + Voxel::CHUNK_SIZE)
	{
		m_render_position.x = camera.GetPosition().x;

		m_render_max.x = m_render_max.x + 1;

		// preallocate memory for chunks
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				const ChunkKey key = { .x = m_render_max.x, .y = y, .z = z };
				_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, nullptr));

				const ChunkKey destroy_key = { .x = m_render_min.x, .y = y, .z = z };
				_destroy_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(destroy_key, nullptr));
			}
		}

		const int32_t z_offset = std::abs(m_render_min.z);
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				_setup_list.at({ m_render_max.x, y, z }) = create_chunk(m_render_max.x, y, z);
				_destroy_list.at({ m_render_min.x, y, z }) = create_chunk(m_render_min.x, y, z);
			}
		}

		m_render_min.x = m_render_min.x + 1;

		render_update.created = m_render_max.x;
		render_update.update_plus = m_render_max.x - 1;
		render_update.update_min = m_render_min.x;
		render_update.updated = true;
	}
	else if (camera.GetPosition().x < m_render_position.x - Voxel::CHUNK_SIZE)
	{
		m_render_position.x = camera.GetPosition().x;

		m_render_min.x = m_render_min.x - 1;

		// preallocate memory for chunks
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				const ChunkKey key = { .x = m_render_min.x, .y = y, .z = z };
				_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, nullptr));

				const ChunkKey destroy_key = { .x = m_render_max.x, .y = y, .z = z };
				_destroy_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(destroy_key, nullptr));
			}
		}

		const int32_t z_offset = std::abs(m_render_min.z);
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				_setup_list.at({ m_render_min.x, y, z }) = create_chunk(m_render_min.x, y, z);
				_destroy_list.at({ m_render_max.x, y, z }) = create_chunk(m_render_max.x, y, z);
			}
		}

		m_render_max.x = m_render_max.x - 1;

		render_update.created = m_render_min.x;
		render_update.update_plus = m_render_min.x + 1;
		render_update.update_min = m_render_max.x;
		render_update.updated = true;
	}

	return render_update;
}

render_update_t ChunkManager::compute_world_update_z(const Camera& camera)
{
	render_update_t render_update;

	if (camera.GetPosition().z > m_render_position.z + Voxel::CHUNK_SIZE)
	{
		m_render_position.z = camera.GetPosition().z;

		m_render_max.z = m_render_max.z + 1;

		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				CreateNewChunk(x, y, m_render_max.z);
				DestroyChunk(x, y, m_render_min.z);
			}
		}

		m_render_min.z = m_render_min.z + 1;

		render_update.created = m_render_max.z;
		render_update.update_plus = m_render_max.z - 1;
		render_update.update_min = m_render_min.z;
		render_update.updated = true;
	}
	else if (camera.GetPosition().z < m_render_position.z - Voxel::CHUNK_SIZE)
	{
		m_render_position.z = camera.GetPosition().z;

		m_render_min.z = m_render_min.z - 1;

		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				CreateNewChunk(x, y, m_render_min.z);
				DestroyChunk(x, y, m_render_max.z);
			}
		}

		m_render_max.z = m_render_max.z - 1;

		render_update.created = m_render_min.z;
		render_update.update_plus = m_render_min.z + 1;
		render_update.update_min = m_render_max.z;
		render_update.updated = true;
	}

	return render_update;
}

void ChunkManager::update_world_x(int32_t create_x, int32_t update_xplus, int32_t update_xmin) const
{
	for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			// m_chunk_map.at({ create_x, y, z })->BuildChunk(m_chunk_map, mp_world, mp_world_mat);
			auto mesh = m_chunk_map.at({ create_x, y, z })->compute_mesh(m_chunk_map);
			if (mesh.has_value())
				m_chunk_map.at({ create_x, y, z })->render_mesh(mesh.value(), *mp_world, mp_world_mat);
			
			m_chunk_map.at({ update_xplus, y, z })->UpdateChunk(m_chunk_map, *mp_world);
			m_chunk_map.at({ update_xmin, y, z })->UpdateChunk(m_chunk_map, *mp_world);
		}
	}
}

void ChunkManager::update_world_z(int32_t create_z, int32_t update_zplus, int32_t update_zmin) const
{
	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			m_chunk_map.at({ x, y, create_z })->BuildChunk(m_chunk_map, mp_world, mp_world_mat);
			m_chunk_map.at({ x, y, update_zplus })->UpdateChunk(m_chunk_map, *mp_world);
			m_chunk_map.at({ x, y, update_zmin })->UpdateChunk(m_chunk_map, *mp_world);
		}
	}
}

void ChunkManager::copy_to_render()
{
	for (auto& [key, chunk] : _setup_list)
	{
		if (chunk)
		{
			m_chunk_map.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, std::move(chunk)));
		}
	}

	for (auto& [key, chunk] : _destroy_list)
	{
		if (chunk)
		{
			m_chunk_map.at(key)->DeleteChunk(mp_world);
			m_chunk_map.erase(key);
		}
	}

	_setup_list.clear();
	_destroy_list.clear();
}

void ChunkManager::CreateNewChunk(int32_t x, int32_t y, int32_t z)
{
	const ChunkKey key = { x, y, z };

	// std::unique_ptr<Chunk> p_chunk = mp_terrain_generator->SetupWorld(x, y, z);
	auto p_chunk = mp_terrain_generator->compute_world_chunk(x, y, z);
	m_chunk_map.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, std::move(p_chunk)));
}

void ChunkManager::DestroyChunk(const int32_t x, const int32_t y, const int32_t z)
{
	const ChunkKey key = { x, y, z };

	m_chunk_map.at(key)->DeleteChunk(mp_world);

	m_chunk_map.erase(key);
}

WorldMenu& ChunkManager::GetMenu()
{
	return m_worldmenu;
}

std::unique_ptr<Chunk> ChunkManager::create_chunk(const int32_t& x, const int32_t& y, const int32_t& z) const
{
	const ChunkKey key = { .x = x, .y = y, .z = z};
	auto p_chunk = mp_terrain_generator->compute_world_chunk(x, y, z);
	return std::move(p_chunk);
}
