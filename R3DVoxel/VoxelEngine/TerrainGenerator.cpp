#include "TerrainGenerator.h"

#include <cmath>

TerrainGenerator::TerrainGenerator() : m_perlin(1337)
{
	//m_perlin.exportppm();
}

TerrainGenerator::~TerrainGenerator()
{
}

std::unique_ptr<Chunk> TerrainGenerator::SetupSphere(const int32_t posx, const int32_t posy, const int32_t posz)
{
    std::unique_ptr<Chunk> p_chunk = std::make_unique<Chunk>(posx, posy, posz);

	for (int32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (int32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
		{
			for (int32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
			{
				if (std::sqrt((x - Voxel::CHUNK_SIZE / 2) * (x - Voxel::CHUNK_SIZE / 2) + (y - Voxel::CHUNK_SIZE / 2) * (y - Voxel::CHUNK_SIZE / 2) + (z - Voxel::CHUNK_SIZE / 2) * (z - Voxel::CHUNK_SIZE / 2)) < Voxel::CHUNK_SIZE / 2)
				{
					p_chunk->SetVoxel(x, y, z);
				}
				
				p_chunk->SetBlockType(x, y, z, TBlock::DEFAULT);
			}
		}
	}

    return std::move(p_chunk);
}

std::unique_ptr<Chunk> TerrainGenerator::SetupWorld(const int32_t posx, const int32_t posy, const int32_t posz)
{
	std::unique_ptr<Chunk> p_chunk = std::make_unique<Chunk>(posx, posy, posz);

	for (int32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (int32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
		{
			for (int32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
			{
				const int8_t x_sign = posx != 0 ? posx / std::abs(posx) : 1;
				const int8_t z_sign = posz != 0 ? posz / std::abs(posz) : 1;

				const int32_t voxel_x_pos = x + posx * Voxel::CHUNK_SIZE;
				const int32_t voxel_z_pos = z + posz * Voxel::CHUNK_SIZE;
				const int32_t voxel_y_pos = y + posy * Voxel::CHUNK_SIZE;

				const float noise_value = m_perlin.perlin(static_cast<float>(voxel_x_pos * x_sign), static_cast<float>(voxel_z_pos * z_sign));
				if (voxel_y_pos < ((noise_value + 1) / 2) * Voxel::CHUNK_SIZE)
				{
					p_chunk->SetVoxel(x, y, z);
				}
				
				p_chunk->SetBlockType(x, y, z, TBlock::DEFAULT);
			}
		}
	}

	return std::move(p_chunk);
}

std::unique_ptr<Chunk> TerrainGenerator::compute_world_chunk(const int32_t posx, const int32_t posy, const int32_t posz) const
{
	auto p_chunk = std::make_unique<Chunk>(posx, posy, posz);

	for (uint32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
			{
				const int8_t x_sign = posx != 0 ? posx / std::abs(posx) : 1;
				const int8_t z_sign = posz != 0 ? posz / std::abs(posz) : 1;

				const int32_t voxel_x_pos = static_cast<int32_t>(x) + posx * static_cast<int32_t>(Voxel::CHUNK_SIZE);
				const int32_t voxel_z_pos = static_cast<int32_t>(z) + posz * static_cast<int32_t>(Voxel::CHUNK_SIZE);
				const int32_t voxel_y_pos = static_cast<int32_t>(y) + posy * static_cast<int32_t>(Voxel::CHUNK_SIZE);

				const float noise_value1 = m_perlin.perlin(static_cast<float>(voxel_x_pos * x_sign), static_cast<float>(voxel_z_pos * z_sign));
				const float noise_value2 = m_perlin.perlin(static_cast<float>(voxel_x_pos * x_sign * 0.5f), static_cast<float>(voxel_z_pos * z_sign * 0.5f));
				const float noise_value3 = m_perlin.perlin(static_cast<float>(voxel_x_pos * x_sign * 0.25f), static_cast<float>(voxel_z_pos * z_sign * 0.25f));

				// Combinaison des valeurs de bruit pour créer des variations de hauteur plus détaillées
				const float combined_noise = (noise_value1 + noise_value2 * 0.5f + noise_value3 * 0.25f) / 1.75f;

				if (voxel_y_pos < ((combined_noise + 1) / 2) * Voxel::CHUNK_SIZE)
				{
					p_chunk->SetVoxel(x, y, z);
				}

				p_chunk->SetBlockType(x, y, z, TBlock::DEFAULT);
			}
		}
	}

	return std::move(p_chunk);
}
