#include "Chunk.h"

#include <iostream>


Chunk::Chunk(std::shared_ptr<Renderer> pRenderer)
{
	m_pRenderer = pRenderer;
	m_pBuffer = std::make_unique<Buffer>();

	m_position = { 0.0f, 0.0f, 0.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };
}


Chunk::~Chunk()
{
}

void Chunk::setup()
{	
	for (uint32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
	{
		for (uint32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
		{
			for (uint32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
			{
				m_activeVox.flip(y + x * Voxel::CHUNK_SIZE + z * Voxel::CHUNK_SIZE_SQR);
			}
		}
	}
}

void Chunk::load()
{
	m_pRenderer->createVerticesBuffer(std::make_shared<std::vector<Voxel::Block>>(m_mesh.vertices), *m_pBuffer.get());
	m_pRenderer->createIndicesBuffer(std::make_shared<std::vector<uint16_t>>(m_mesh.indices), *m_pBuffer.get());
}

void Chunk::createMesh()
{
	for (uint32_t x = 0; x < Voxel::CHUNK_SIZE; x++)
	{
		for (uint32_t z = 0; z < Voxel::CHUNK_SIZE; z++)
		{
			for (uint32_t y = 0; y < Voxel::CHUNK_SIZE; y++)
			{
				if (m_activeVox[y + z * Voxel::CHUNK_SIZE + x * Voxel::CHUNK_SIZE_SQR])
				{
					//vertex
					glm::vec3 p1(m_position.x + x - Voxel::BLOCK_RENDER_SIZE, m_position.y + y - Voxel::BLOCK_RENDER_SIZE, m_position.z + z + Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p2(m_position.x + x + Voxel::BLOCK_RENDER_SIZE, m_position.y + y - Voxel::BLOCK_RENDER_SIZE, m_position.z + z + Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p3(m_position.x + x - Voxel::BLOCK_RENDER_SIZE, m_position.y + y + Voxel::BLOCK_RENDER_SIZE, m_position.z + z + Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p4(m_position.x + x + Voxel::BLOCK_RENDER_SIZE, m_position.y + y + Voxel::BLOCK_RENDER_SIZE, m_position.z + z + Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p5(m_position.x + x + Voxel::BLOCK_RENDER_SIZE, m_position.y + y - Voxel::BLOCK_RENDER_SIZE, m_position.z + z - Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p6(m_position.x + x - Voxel::BLOCK_RENDER_SIZE, m_position.y + y - Voxel::BLOCK_RENDER_SIZE, m_position.z + z - Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p7(m_position.x + x + Voxel::BLOCK_RENDER_SIZE, m_position.y + y + Voxel::BLOCK_RENDER_SIZE, m_position.z + z - Voxel::BLOCK_RENDER_SIZE);
					glm::vec3 p8(m_position.x + x - Voxel::BLOCK_RENDER_SIZE, m_position.y + y + Voxel::BLOCK_RENDER_SIZE, m_position.z + z - Voxel::BLOCK_RENDER_SIZE);

					//texture
					glm::vec2 t1(0.0f, 0.0f);
					glm::vec2 t2(0.0f, 2.0f);
					glm::vec2 t3(2.0f, 0.0f);
					glm::vec2 t4(2.0f, 2.0f);

					//color
					glm::vec3 c1(1.0f, 0.0f, 0.0f);
					glm::vec3 c2(1.0f, 1.0f, 0.0f);
					glm::vec3 c3(0.0f, 0.0f, 1.0f);
					glm::vec3 c4(1.0f, 1.0f, 1.0f);

					uint32_t index_1, index_2, index_3, index_4, index_5, index_6, index_7, index_8;


					//declare vertex
					index_1 = m_mesh.addVertex(p1, c1, t1);
					index_2 = m_mesh.addVertex(p2, c2, t2);
					index_3 = m_mesh.addVertex(p3, c3, t3);
					index_4 = m_mesh.addVertex(p4, c4, t4);
					index_5 = m_mesh.addVertex(p5, c1, t1);
					index_6 = m_mesh.addVertex(p6, c2, t2);
					index_7 = m_mesh.addVertex(p7, c3, t3);
					index_8 = m_mesh.addVertex(p8, c4, t4);

					//front
					m_mesh.addIndices(index_1, index_2, index_4);
					m_mesh.addIndices(index_4, index_3, index_1);

					//back
					m_mesh.addIndices(index_5, index_6, index_8);
					m_mesh.addIndices(index_8, index_7, index_5);

					//right
					m_mesh.addIndices(index_2, index_5, index_7);
					m_mesh.addIndices(index_7, index_4, index_2);

					//left
					m_mesh.addIndices(index_6, index_1, index_3);
					m_mesh.addIndices(index_3, index_8, index_6);

					//top
					m_mesh.addIndices(index_3, index_4, index_7);
					m_mesh.addIndices(index_7, index_8, index_3);

					//bot
					m_mesh.addIndices(index_6, index_5, index_2);
					m_mesh.addIndices(index_2, index_1, index_6);
				}
			}
		}
	}
}

void Chunk::destroyMesh()
{
	if (m_pBuffer != nullptr)
	{
		m_pRenderer->destroyBuffers(*m_pBuffer.get());
	}
	else
	{
		Logger::registerError("Buffer was destroy or empty prior its controlled destruction!");
	}
}

size_t Chunk::getSizeIndices()
{
	return m_mesh.indices.size();
}

glm::vec3 Chunk::getPosition()
{
	return m_position;
}

glm::vec3 Chunk::getRotation()
{
	return m_rotation;
}

Buffer & Chunk::getBuffers()
{
	return *m_pBuffer.get();
}

void Chunk::setPosition(float x, float y, float z)
{
	m_position.x = x * Voxel::CHUNK_SIZE;
	m_position.y = y * Voxel::CHUNK_SIZE;
	m_position.z = z * Voxel::CHUNK_SIZE;
}
