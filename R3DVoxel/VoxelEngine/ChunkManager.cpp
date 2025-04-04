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
	std::barrier sync(num_threads, [this, &chunks]() noexcept { _chunk_map = std::move(chunks); });

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
				_chunk_map.at({ x, y, z })->BuildChunk(_chunk_map, mp_world, mp_world_mat);
			}
		}
	}
}

std::optional<render_update_t> ChunkManager::compute_world_update_x(const Camera& camera)
{
	if (camera.GetPosition().x > m_render_position.x + Voxel::CHUNK_SIZE)
	{
		return update_x_forward(camera);
	}
	else if (camera.GetPosition().x < m_render_position.x - Voxel::CHUNK_SIZE)
	{
		return update_x_backward(camera);
	}

	return {};
}

std::optional<render_update_t> ChunkManager::compute_world_update_z(const Camera& camera)
{
	if (camera.GetPosition().z > m_render_position.z + Voxel::CHUNK_SIZE)
	{
		return update_z_forward(camera);
	}
	else if (camera.GetPosition().z < m_render_position.z - Voxel::CHUNK_SIZE)
	{
		return update_z_backward(camera);
	}

	return {};
}

std::map<ChunkKey, Geometry> ChunkManager::compute_meshes(std::optional<render_update_t> x, std::optional<render_update_t> z)
{
	std::map<ChunkKey, Geometry> meshes;

	if (x.has_value())
	{
		const auto& x_update = x.value();
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				Chunk* new_chunk = _setup_list.at({ x_update.created, y, z }).get();
				if (new_chunk->is_active())
				{
					const ChunkKey key = { x_update.created, y, z };
					meshes.emplace(std::pair<ChunkKey, Geometry>(key, new_chunk->compute_mesh(_setup_list)));
				}

				Chunk* front_chunk = _setup_list.at({ x_update.update_plus, y, z }).get();
				if (front_chunk->is_active() || front_chunk->mesh_id() != -1)
				{
					const ChunkKey key = { x_update.update_plus, y, z };
					meshes.emplace(std::pair<ChunkKey, Geometry>(key, front_chunk->compute_mesh(_setup_list)));
				}

				Chunk* back_chunk = _setup_list.at({ x_update.update_min, y, z }).get();
				if (back_chunk->is_active() || back_chunk->mesh_id() != -1)
				{
					const ChunkKey key = { x_update.update_min, y, z };
					meshes.emplace(std::pair<ChunkKey, Geometry>(key, back_chunk->compute_mesh(_setup_list)));
				}
			}
		}
	}

	if (z.has_value())
	{
		const auto& z_update = z.value();
		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				Chunk* new_chunk = _setup_list.at({ x, y, z_update.created }).get();
				if (new_chunk->is_active())
				{
					const ChunkKey key = { x, y, z_update.created };
					meshes.emplace(std::pair<ChunkKey, Geometry>(key, new_chunk->compute_mesh(_setup_list)));
				}
				Chunk* front_chunk = _setup_list.at({ x, y, z_update.update_plus }).get();
				if (front_chunk->is_active() || front_chunk->mesh_id() != -1)
				{
					const ChunkKey key = { x, y, z_update.update_plus };
					meshes.emplace(std::pair<ChunkKey, Geometry>(key, front_chunk->compute_mesh(_setup_list)));
				}
				Chunk* back_chunk = _setup_list.at({ x, y, z_update.update_min }).get();
				if (back_chunk->is_active() || back_chunk->mesh_id() != -1)
				{
					const ChunkKey key = { x, y, z_update.update_min };
					meshes.emplace(std::pair<ChunkKey, Geometry>(key, back_chunk->compute_mesh(_setup_list)));
				}
			}
		}
	}

	return meshes;
}

void ChunkManager::render_meshes(const std::map<ChunkKey, Geometry>& meshes, std::optional<render_update_t> x, std::optional<render_update_t> z)
{
	if (x.has_value())
	{
		const auto& update = x.value();
		for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				const ChunkKey create_key = { update.created, y, z };
				Chunk* new_chunk = _chunk_map.at(create_key).get();
				if (new_chunk->is_active())
				{
					auto mesh = meshes.at(create_key);
					new_chunk->render_mesh(mesh, *mp_world, mp_world_mat);
				}

				const ChunkKey front_key = { update.update_plus , y, z };
				Chunk* front_chunk = _chunk_map.at(front_key).get();
				if (front_chunk->is_active() || front_chunk->mesh_id() != -1)
				{
					auto mesh = meshes.at(front_key);
					front_chunk->update_mesh(mesh, *mp_world);
				}

				const ChunkKey back_key = { update.update_min, y, z };
				Chunk* back_chunk = _chunk_map.at(back_key).get();
				if (back_chunk->is_active() || back_chunk->mesh_id() != -1)
				{
					auto mesh = meshes.at(back_key);
					back_chunk->update_mesh(mesh, *mp_world);
				}
			}
		}
	}

	if (z.has_value())
	{
		const auto& update = z.value();
		for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				const ChunkKey create_key = { x, y, update.created };
				Chunk* new_chunk = _chunk_map.at(create_key).get();
				if (new_chunk->is_active())
				{
					auto mesh = meshes.at(create_key);
					new_chunk->render_mesh(mesh, *mp_world, mp_world_mat);
				}

				const ChunkKey front_key = { x, y, update.update_plus };
				Chunk* front_chunk = _chunk_map.at(front_key).get();
				if (front_chunk->is_active() || front_chunk->mesh_id() != -1)
				{
					auto mesh = meshes.at(front_key);
					front_chunk->update_mesh(mesh, *mp_world);
				}

				const ChunkKey back_key = { x, y, update.update_min };
				Chunk* back_chunk = _chunk_map.at(back_key).get();
				if (back_chunk->is_active() || back_chunk->mesh_id() != -1)
				{
					auto mesh = meshes.at(back_key);
					back_chunk->update_mesh(mesh, *mp_world);
				}
			}
		}
	}
}

void ChunkManager::copy_to_render()
{
	for (auto& [key, chunk] : _setup_list)
	{
		if (chunk && !_chunk_map.contains(key)) // should make a simple list of keys of created chunks to prevent lookup
		{
			_chunk_map.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, std::move(chunk)));
		}
	}

	for (auto& [key, chunk] : _destroy_list) // maybe store pointers to be deleted (prevents lookup in map)
	{
		if (chunk)
		{
			_chunk_map.at(key)->DeleteChunk(mp_world);
			_chunk_map.erase(key);
		}
	}

	_setup_list.clear();
	_destroy_list.clear();
}

WorldMenu& ChunkManager::GetMenu()
{
	return m_worldmenu;
}

render_update_t ChunkManager::update_x_forward(const Camera& camera)
{
	const int64_t num_threads = (m_render_max.z - m_render_min.z) + 2;
	std::barrier sync(num_threads);

	m_render_position.x = camera.GetPosition().x;

	m_render_max.x = m_render_max.x + 1;

	// preallocate memory for chunks
	for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			const ChunkKey new_key = { .x = m_render_max.x, .y = y, .z = z };
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(new_key, nullptr));

			const ChunkKey front_key = { .x = m_render_max.x - 1, .y = y, .z = z };
			auto copy_front = std::make_unique<Chunk>(*_chunk_map.at(front_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(front_key, std::move(copy_front)));

			const ChunkKey back_key = { .x = m_render_min.x + 1, .y = y, .z = z };
			auto copy_back = std::make_unique<Chunk>(*_chunk_map.at(back_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(back_key, std::move(copy_back)));

			const ChunkKey destroy_key = { .x = m_render_min.x, .y = y, .z = z };
			_destroy_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(destroy_key, nullptr));
		}
	}

	for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
	{
		auto task = [z, &sync, this]()
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				_setup_list.at({ m_render_max.x, y, z }) = create_chunk(m_render_max.x, y, z);
				_destroy_list.at({ m_render_min.x, y, z }) = create_chunk(m_render_min.x, y, z);
			}

			sync.arrive_and_drop();
		};
		_tpool.enqueue(task);
	}

	sync.arrive_and_wait(); // wait for all chunks to be created

	m_render_min.x = m_render_min.x + 1;

	render_update_t render_update;
	render_update.created = m_render_max.x;
	render_update.update_plus = m_render_max.x - 1;
	render_update.update_min = m_render_min.x;
	render_update.updated = true;

	return render_update;
}

render_update_t ChunkManager::update_x_backward(const Camera& camera)
{
	m_render_position.x = camera.GetPosition().x;

	m_render_min.x = m_render_min.x - 1;

	// preallocate memory for chunks
	for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			const ChunkKey new_key = { .x = m_render_min.x, .y = y, .z = z };
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(new_key, nullptr));

			const ChunkKey front_key = { .x = m_render_min.x + 1, .y = y, .z = z };
			auto copy_front = std::make_unique<Chunk>(*_chunk_map.at(front_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(front_key, std::move(copy_front)));

			const ChunkKey back_key = { .x = m_render_max.x - 1, .y = y, .z = z };
			auto copy_back = std::make_unique<Chunk>(*_chunk_map.at(back_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(back_key, std::move(copy_back)));

			const ChunkKey destroy_key = { .x = m_render_max.x, .y = y, .z = z };
			_destroy_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(destroy_key, nullptr));
		}
	}

	const int64_t num_threads = (m_render_max.z - m_render_min.z) + 2;
	std::barrier sync(num_threads);

	for (int32_t z = m_render_min.z; z <= m_render_max.z; z++)
	{
		auto task = [z, &sync, this]()
		{
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				_setup_list.at({ m_render_min.x, y, z }) = create_chunk(m_render_min.x, y, z);
				_destroy_list.at({ m_render_max.x, y, z }) = create_chunk(m_render_max.x, y, z);
			}

			sync.arrive_and_drop();
		};

		_tpool.enqueue(task);
	}

	sync.arrive_and_wait(); // wait for all chunks to be created

	m_render_max.x = m_render_max.x - 1;

	render_update_t render_update;
	render_update.created = m_render_min.x;
	render_update.update_plus = m_render_min.x + 1;
	render_update.update_min = m_render_max.x;
	render_update.updated = true;

	return render_update;
}

render_update_t ChunkManager::update_z_forward(const Camera& camera)
{
	const int64_t num_threads = (m_render_max.z - m_render_min.z) + 2;
	std::barrier sync(num_threads);

	m_render_position.z = camera.GetPosition().z;

	m_render_max.z = m_render_max.z + 1;

	// preallocate memory for chunks
	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			const ChunkKey new_key = { .x = x, .y = y, .z = m_render_max.z };
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(new_key, nullptr));

			const ChunkKey front_key = { .x = x, .y = y, .z = m_render_max.z - 1 };
			auto copy_front = std::make_unique<Chunk>(*_chunk_map.at(front_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(front_key, std::move(copy_front)));

			const ChunkKey back_key = { .x = x, .y = y, .z = m_render_min.z + 1 };
			auto copy_back = std::make_unique<Chunk>(*_chunk_map.at(back_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(back_key, std::move(copy_back)));

			const ChunkKey destroy_key = { .x = x, .y = y, .z = m_render_min.z };
			_destroy_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(destroy_key, nullptr));
		}
	}

	// compute chunks in parallel
	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		auto task = [x, &sync, this]() {
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				_setup_list.at({ x, y, m_render_max.z }) = create_chunk(x, y, m_render_max.z);
				_destroy_list.at({ x, y, m_render_min.z }) = create_chunk(x, y, m_render_min.z);
			}
			sync.arrive_and_drop();
		};
		_tpool.enqueue(task);
	}

	sync.arrive_and_wait(); // wait for all chunks to be created

	m_render_min.z = m_render_min.z + 1;

	render_update_t render_update;
	render_update.created = m_render_max.z;
	render_update.update_plus = m_render_max.z - 1;
	render_update.update_min = m_render_min.z;
	render_update.updated = true;

	return render_update;
}

render_update_t ChunkManager::update_z_backward(const Camera& camera)
{
	const int64_t num_threads = (m_render_max.z - m_render_min.z) + 2;
	std::barrier sync(num_threads);

	m_render_position.z = camera.GetPosition().z;

	m_render_min.z = m_render_min.z - 1;

	// preallocate memory for chunks
	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
		{
			const ChunkKey new_key = { .x = x, .y = y, .z = m_render_min.z };
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(new_key, nullptr));

			const ChunkKey front_key = { .x = x, .y = y, .z = m_render_min.z + 1 };
			auto copy_front = std::make_unique<Chunk>(*_chunk_map.at(front_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(front_key, std::move(copy_front)));

			const ChunkKey back_key = { .x = x, .y = y, .z = m_render_max.z - 1 };
			auto copy_back = std::make_unique<Chunk>(*_chunk_map.at(back_key));
			_setup_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(back_key, std::move(copy_back)));

			const ChunkKey destroy_key = { .x = x, .y = y, .z = m_render_max.z };
			_destroy_list.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(destroy_key, nullptr));
		}
	}

	// compute chunks in parallel
	for (int32_t x = m_render_min.x; x <= m_render_max.x; x++)
	{
		auto task = [x, &sync, this]() {
			for (int32_t y = m_render_min.y; y <= m_render_max.y; y++)
			{
				_setup_list.at({ x, y, m_render_min.z }) = create_chunk(x, y, m_render_min.z);
				_destroy_list.at({ x, y, m_render_max.z }) = create_chunk(x, y, m_render_max.z);
			}

			sync.arrive_and_drop();
		};

		_tpool.enqueue(task);
	}

	sync.arrive_and_wait(); // wait for all chunks to be created

	m_render_max.z = m_render_max.z - 1;

	render_update_t render_update;
	render_update.created = m_render_min.z;
	render_update.update_plus = m_render_min.z + 1;
	render_update.update_min = m_render_max.z;
	render_update.updated = true;

	return render_update;
}

std::unique_ptr<Chunk> ChunkManager::create_chunk(const int32_t& x, const int32_t& y, const int32_t& z) const
{
	const ChunkKey key = { .x = x, .y = y, .z = z};
	auto p_chunk = mp_terrain_generator->compute_world_chunk(x, y, z);
	return std::move(p_chunk);
}
