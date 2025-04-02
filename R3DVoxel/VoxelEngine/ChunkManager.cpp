#include "ChunkManager.h"

#include <utility>
#include <barrier>

ChunkManager::ChunkManager(std::shared_ptr<GameObject> pworld, std::shared_ptr<Material> p_world_mat, std::shared_ptr<Camera> p_camera) :
	mp_world(std::move(pworld)), mp_world_mat(std::move(p_world_mat)), m_worldmenu(m_load_radius)
{
	mp_terrain_generator = std::make_unique<TerrainGenerator>();
	m_render_position = p_camera->GetPosition();
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
		std::thread t([&chunks, x, &sync, this]() { 
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
			
				for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
				{
					// CreateNewChunk(x, y, z);
					chunks.at({ x, y, z }) = create_chunk(x, y, z);
				}
			}

			sync.arrive_and_wait();
		});
		t.detach();
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

void ChunkManager::UpdateWorld(std::shared_ptr<Scene> p_scene, std::shared_ptr<Camera> p_camera)
{
	bool scene_x_need_update = false;

	int32_t create_x = -1;
	int32_t update_xplus = -1;
	int32_t update_xmin = -1;
	if (p_camera->GetPosition().x > m_render_position.x + Voxel::CHUNK_SIZE)
	{
		m_render_position.x = p_camera->GetPosition().x;

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
	else if (p_camera->GetPosition().x < m_render_position.x - Voxel::CHUNK_SIZE)
	{
		m_render_position.x = p_camera->GetPosition().x;

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
	if (p_camera->GetPosition().z > m_render_position.z + Voxel::CHUNK_SIZE)
	{
		m_render_position.z = p_camera->GetPosition().z;

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
	else if (p_camera->GetPosition().z < m_render_position.z - Voxel::CHUNK_SIZE)
	{
		m_render_position.z = p_camera->GetPosition().z;

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
				m_chunk_map.at({ update_xplus, y, z })->UpdateChunk(m_chunk_map, mp_world);
				m_chunk_map.at({ update_xmin, y, z })->UpdateChunk(m_chunk_map, mp_world);
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
				m_chunk_map.at({ x, y, update_zplus })->UpdateChunk(m_chunk_map, mp_world);
				m_chunk_map.at({ x, y, update_zmin })->UpdateChunk(m_chunk_map, mp_world);
			}
		}
	}

	if (scene_x_need_update || scene_z_need_update)
		p_scene->ToUpdate();

	Watcher::WatchPosition("render position", m_render_position);
	Watcher::WatchPosition("render max", m_render_max);
	Watcher::WatchPosition("render min", m_render_min);
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
