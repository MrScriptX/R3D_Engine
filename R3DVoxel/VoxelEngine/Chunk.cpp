#include "Chunk.h"

Chunk::Chunk(const int32_t posx, const int32_t posy, const int32_t posz)
{
	m_position = { posx, posy, posz };
	m_mesh_id = -1;

	m_active_voxel.reset();
	m_visible_voxel = { false };
}

void Chunk::BuildChunk(const std::map<ChunkKey, std::unique_ptr<Chunk>>& chunk_map, std::shared_ptr<GameObject> world, std::shared_ptr<Material> mat)
{
	if (m_active_voxel == false)
		return;

	m_visible_voxel = { true };

	Geometry mesh;
	for (uint32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
			{
				const uint32_t voxel = x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR;
				if (m_active_voxel[voxel])
				{
					if (x > 0)
					{
						m_visible_voxel[voxel].xneg = m_active_voxel[(x - 1) + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x) - 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].xneg = chunk_map.at({ static_cast<int32_t>(m_position.x) - 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) })->GetVoxel(x + Voxel::CHUNK_SIZE - 1, y, z);
					}

					if (x < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].xpos = m_active_voxel[(x + 1) + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x) + 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) }) != chunk_map.end())
					{
						m_visible_voxel[voxel].xpos = chunk_map.at({ static_cast<int32_t>(m_position.x) + 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) })->GetVoxel(x - Voxel::CHUNK_SIZE + 1, y, z);
					}

					if (y > 0)
					{
						m_visible_voxel[voxel].yneg = m_active_voxel[x + (y - 1) * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}

					if (y < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].ypos = m_active_voxel[x + (y + 1) * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}

					if (z > 0)
					{
						m_visible_voxel[voxel].zneg = m_active_voxel[x + y * Voxel::CHUNK_SIZE + (z - 1) * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) - 1 }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].zneg = chunk_map.at({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) - 1 })->GetVoxel(x, y, z + Voxel::CHUNK_SIZE - 1);
					}

					if (z < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].zpos = m_active_voxel[x + y * Voxel::CHUNK_SIZE + (z + 1) * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) + 1 }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].zpos = chunk_map.at({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) + 1 })->GetVoxel(x, y, z - Voxel::CHUNK_SIZE + 1); // faces are invisible because they are removed before, should make 2 lists
					}

					CreateCube(mesh, x, y, z);
				}
			}
		}
	}

	m_mesh_id = world->LoadMesh(mesh.vertices, mesh.indices);
	world->bindMatToMesh(m_mesh_id, mat);
}

void Chunk::UpdateChunk(const std::map<ChunkKey, std::unique_ptr<Chunk>>& chunk_map, GameObject& world)
{
	// mesh should be updated only m_mesh_id != -1 otherwise, it should be built
	if (m_active_voxel == false || m_mesh_id == -1)
		return;

	m_visible_voxel = { true };

	Geometry mesh;
	for (uint32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
			{
				const uint32_t voxel = x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR;
				if (m_active_voxel[voxel])
				{
					if (x > 0)
					{
						m_visible_voxel[voxel].xneg = m_active_voxel[(x - 1) + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x) - 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].xneg = chunk_map.at({ static_cast<int32_t>(m_position.x) - 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) })->GetVoxel(x + Voxel::CHUNK_SIZE - 1, y, z);
					}

					if (x < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].xpos = m_active_voxel[(x + 1) + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x) + 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) }) != chunk_map.end())
					{
						m_visible_voxel[voxel].xpos = chunk_map.at({ static_cast<int32_t>(m_position.x) + 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) })->GetVoxel(x - Voxel::CHUNK_SIZE + 1, y, z);
					}

					if (y > 0)
					{
						m_visible_voxel[voxel].yneg = m_active_voxel[x + (y - 1) * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}

					if (y < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].ypos = m_active_voxel[x + (y + 1) * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}

					if (z > 0)
					{
						m_visible_voxel[voxel].zneg = m_active_voxel[x + y * Voxel::CHUNK_SIZE + (z - 1) * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) - 1 }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].zneg = chunk_map.at({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) - 1 })->GetVoxel(x, y, z + Voxel::CHUNK_SIZE - 1);
					}

					if (z < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].zpos = m_active_voxel[x + y * Voxel::CHUNK_SIZE + (z + 1) * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) + 1 }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].zpos = chunk_map.at({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) + 1 })->GetVoxel(x, y, z - Voxel::CHUNK_SIZE + 1); // faces are invisible because they are removed before, should make 2 lists
					}

					CreateCube(mesh, x, y, z);
				}
			}
		}
	}

	world.UpdateMesh(m_mesh_id, mesh.vertices, mesh.indices);
}

void Chunk::DeleteChunk(std::shared_ptr<GameObject> world)
{
	if(m_mesh_id != -1)
		world->RemoveMesh(m_mesh_id);
}

Geometry Chunk::compute_mesh(const std::map<ChunkKey, std::unique_ptr<Chunk>>& chunk_map)
{
	/*if (m_active_voxel == false)
		return {};*/

	m_visible_voxel = { true };

	Geometry mesh;
	for (uint32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
			{
				const uint32_t voxel = x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR;
				if (m_active_voxel[voxel])
				{
					if (x > 0)
					{
						m_visible_voxel[voxel].xneg = m_active_voxel[(x - 1) + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x) - 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].xneg =
							chunk_map.at({ static_cast<int32_t>(m_position.x) - 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) })
								->GetVoxel(x + Voxel::CHUNK_SIZE - 1, y, z);
					}

					if (x < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].xpos = m_active_voxel[(x + 1) + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x) + 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) }) != chunk_map.end())
					{
						m_visible_voxel[voxel].xpos =
							chunk_map.at({ static_cast<int32_t>(m_position.x) + 1, static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) })
								->GetVoxel(x - Voxel::CHUNK_SIZE + 1, y, z);
					}

					if (y > 0)
					{
						m_visible_voxel[voxel].yneg = m_active_voxel[x + (y - 1) * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}

					if (y < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].ypos = m_active_voxel[x + (y + 1) * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
					}

					if (z > 0)
					{
						m_visible_voxel[voxel].zneg = m_active_voxel[x + y * Voxel::CHUNK_SIZE + (z - 1) * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) - 1 }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].zneg =
						    chunk_map.at({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) - 1 })
						        ->GetVoxel(x, y, z + Voxel::CHUNK_SIZE - 1);
					}

					if (z < Voxel::CHUNK_SIZE - 1)
					{
						m_visible_voxel[voxel].zpos = m_active_voxel[x + y * Voxel::CHUNK_SIZE + (z + 1) * Voxel::CHUNK_SIZE_SQR];
					}
					else if (chunk_map.find({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) + 1 }) != chunk_map.end())
					{
						// check if face is hidden by other chunk
						m_visible_voxel[voxel].zpos =
						    chunk_map.at({ static_cast<int32_t>(m_position.x), static_cast<int32_t>(m_position.y), static_cast<int32_t>(m_position.z) + 1 })
						        ->GetVoxel(x, y, z - Voxel::CHUNK_SIZE + 1); // faces are invisible because they are removed before, should make 2 lists
					}

					CreateCube(mesh, x, y, z);
				}
			}
		}
	}

	return std::move(mesh);
}

void Chunk::render_mesh(const Geometry& mesh, GameObject& world, std::shared_ptr<Material> mat)
{
	m_mesh_id = world.LoadMesh(mesh.vertices, mesh.indices);
	world.bindMatToMesh(m_mesh_id, mat);
}

void Chunk::update_mesh(const Geometry& mesh, GameObject& world)
{
	world.UpdateMesh(m_mesh_id, mesh.vertices, mesh.indices);
}

void Chunk::SetVoxel(const uint32_t x, const uint32_t y, const uint32_t z)
{
	m_active_voxel.set(x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR, true);
}

void Chunk::SetBlockType(const uint32_t x, const uint32_t y, const uint32_t z, const TBlock type)
{
	m_blocktypes.at(x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR) = type;
}

bool Chunk::GetVoxel(const uint32_t x, const uint32_t y, const uint32_t z) const
{
	return m_active_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR];
}

bool Chunk::is_active() const
{
	return m_active_voxel.any();
}

glm::vec3 Chunk::GetPosition() const
{
	return m_position;
}

int32_t Chunk::mesh_id() const
{
	return m_mesh_id;
}

void Chunk::CreateCube(Geometry& mesh, uint32_t x, uint32_t y, uint32_t z)
{
	//vertex
	glm::vec3 p1(m_position.x * Voxel::CHUNK_SIZE + x - Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y - Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z - Voxel::VOXEL_SIZE);
	glm::vec3 p2(m_position.x * Voxel::CHUNK_SIZE + x + Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y - Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z - Voxel::VOXEL_SIZE);
	glm::vec3 p3(m_position.x * Voxel::CHUNK_SIZE + x - Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y + Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z - Voxel::VOXEL_SIZE);
	glm::vec3 p4(m_position.x * Voxel::CHUNK_SIZE + x + Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y + Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z - Voxel::VOXEL_SIZE);
	glm::vec3 p5(m_position.x * Voxel::CHUNK_SIZE + x - Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y - Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z + Voxel::VOXEL_SIZE);
	glm::vec3 p6(m_position.x * Voxel::CHUNK_SIZE + x + Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y - Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z + Voxel::VOXEL_SIZE);
	glm::vec3 p7(m_position.x * Voxel::CHUNK_SIZE + x - Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y + Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z + Voxel::VOXEL_SIZE);
	glm::vec3 p8(m_position.x * Voxel::CHUNK_SIZE + x + Voxel::VOXEL_SIZE, m_position.y * Voxel::CHUNK_SIZE + y + Voxel::VOXEL_SIZE, m_position.z * Voxel::CHUNK_SIZE + z + Voxel::VOXEL_SIZE);

	//texture
	glm::vec2 t1(0.0f, 0.0f);
	glm::vec2 t2(0.0f, 2.0f);
	glm::vec2 t3(2.0f, 0.0f);
	glm::vec2 t4(2.0f, 2.0f);

	//color
	glm::vec3 color;
	switch (m_blocktypes[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR])
	{
	case TBlock::GRASS:
		color = { 0.0f, 1.0f, 0.0f };
		break;
	case TBlock::DEFAULT:
		color = { 1.0f, 1.0f, 1.0f };
		break;
	default:
		color = { 1.0f, 0.3f, 0.0f };
		break;
	}	

	// build mesh
	//front
	if (!m_visible_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR].zneg)
	{
		const uint32_t index_0 = mesh.addVertex(p1, color, t1);
		const uint32_t index_1 = mesh.addVertex(p2, color, t2);
		const uint32_t index_2 = mesh.addVertex(p3, color, t3);
		const uint32_t index_3 = mesh.addVertex(p4, color, t4);

		mesh.addIndices(index_0, index_2, index_1);
		mesh.addIndices(index_1, index_2, index_3);
	}

	//back
	if (!m_visible_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR].zpos)
	{
		uint32_t index_4 = mesh.addVertex(p5, color, t1);
		uint32_t index_5 = mesh.addVertex(p6, color, t2);
		uint32_t index_6 = mesh.addVertex(p7, color, t3);
		uint32_t index_7 = mesh.addVertex(p8, color, t4);

		mesh.addIndices(index_5, index_7, index_4);
		mesh.addIndices(index_4, index_7, index_6);
	}

	//right
	if (!m_visible_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR].xpos)
	{
		uint32_t index_1_1 = mesh.addVertex(p2, color, t2);
		uint32_t index_3_1 = mesh.addVertex(p4, color, t4);
		uint32_t index_5_1 = mesh.addVertex(p6, color, t2);
		uint32_t index_7_1 = mesh.addVertex(p8, color, t4);

		mesh.addIndices(index_1_1, index_3_1, index_5_1);
		mesh.addIndices(index_5_1, index_3_1, index_7_1);
	}

	//left
	if (!m_visible_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR].xneg)
	{
		uint32_t index_0_1 = mesh.addVertex(p1, color, t1);
		uint32_t index_2_1 = mesh.addVertex(p3, color, t3);
		uint32_t index_4_1 = mesh.addVertex(p5, color, t1);
		uint32_t index_6_1 = mesh.addVertex(p7, color, t3);

		mesh.addIndices(index_4_1, index_6_1, index_0_1);
		mesh.addIndices(index_0_1, index_6_1, index_2_1);
	}

	//top
	if (!m_visible_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR].ypos)
	{
		uint32_t index_2_2 = mesh.addVertex(p3, color, t3);
		uint32_t index_3_2 = mesh.addVertex(p4, color, t4);
		uint32_t index_6_2 = mesh.addVertex(p7, color, t3);
		uint32_t index_7_2 = mesh.addVertex(p8, color, t4);

		mesh.addIndices(index_2_2, index_6_2, index_3_2);
		mesh.addIndices(index_3_2, index_6_2, index_7_2);
	}

	//bot
	if (!m_visible_voxel[x + y * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR].yneg)
	{
		uint32_t index_0_2 = mesh.addVertex(p1, color, t1);
		uint32_t index_1_2 = mesh.addVertex(p2, color, t2);
		uint32_t index_4_2 = mesh.addVertex(p5, color, t1);
		uint32_t index_5_2 = mesh.addVertex(p6, color, t2);

		mesh.addIndices(index_4_2, index_0_2, index_5_2);
		mesh.addIndices(index_5_2, index_0_2, index_1_2);
	}
}
