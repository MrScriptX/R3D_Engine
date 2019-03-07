#include "ChunkManager.h"



ChunkManager::ChunkManager(std::shared_ptr<Renderer> pRenderer, std::shared_ptr<Player> pPlayer)
{
	m_pRenderer = pRenderer;
	m_pPlayer = pPlayer;

	m_needRebuild = true;
	m_renderPosition = { 0.0f, 0.0f, 0.0f };

	minXPos = 0;
	minZPos = 0;
	maxXPos = Voxel::CHUNK_SIZE;
	maxZPos = Voxel::CHUNK_SIZE;
}


ChunkManager::~ChunkManager()
{
	for (std::map<ChunkKey, std::unique_ptr<Chunk>>::iterator it = m_chunkMap.begin(); it != m_chunkMap.end(); it++)
	{
		it->second->destroyMesh();
	}
}

void ChunkManager::loadChunks()
{
	for (std::map<ChunkKey, std::unique_ptr<Chunk>>::iterator it = m_chunkMap.begin(); it != m_chunkMap.end(); it++)
	{
		it->second->load();
	}
}

void ChunkManager::renderChunks(VkCommandBuffer& commandBuffer, Pipeline& pipeline)
{
	for (std::map<ChunkKey, std::unique_ptr<Chunk>>::iterator it = m_chunkMap.begin(); it != m_chunkMap.end(); it++)
	{
		m_pRenderer->recordDrawCommands(commandBuffer, pipeline, it->second->getBuffers(), it->second->getSizeIndices());
	}
}

void ChunkManager::createNewChunk(int32_t x, int32_t y, int32_t z)
{
	ChunkKey key = { x, y, z };//define key

	//create new chunk
	std::unique_ptr<Chunk> pChunk = std::make_unique<Chunk>(m_pRenderer);
	pChunk->setPosition(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	pChunk->setup();

	m_chunkMap.insert(std::pair<ChunkKey, std::unique_ptr<Chunk>>(key, std::move(pChunk)));

	m_chunkMap.at(key)->createMesh();
}

void ChunkManager::unloadChunk(int32_t x, int32_t y, int32_t z)
{
	//vkFreeCommandBuffers() or vkResetCommandBuffers()

	ChunkKey key = { x, y, z };
	m_chunkMap.find(key)->second->destroyMesh();
	m_chunkMap.erase(key);
}

void ChunkManager::loadNewChunk(int32_t x, int32_t y, int32_t z)
{
	ChunkKey key = { x, y, z };

	m_chunkMap.find(key)->second->load();
}

Buffer & ChunkManager::getBuffers(int32_t x, int32_t y, int32_t z)
{
	ChunkKey key = { x, y, z };

	return m_chunkMap.find(key)->second->getBuffers();
}

size_t ChunkManager::getIndicesSize(int32_t x, int32_t y, int32_t z)
{
	ChunkKey key = { x, y, z };

	return m_chunkMap.find(key)->second->getSizeIndices();
}

bool ChunkManager::needRebuild()
{
	return m_needRebuild;
}

void ChunkManager::setRebuild(bool isRebuilt)
{
	m_needRebuild = isRebuilt;
}

void ChunkManager::update()
{
	if (m_pPlayer->getPosition().x > m_renderPosition.x + Voxel::CHUNK_SIZE)
	{
		m_renderPosition.x = m_pPlayer->getPosition().x;

		for (int32_t z = minZPos; z < maxZPos; z++)
		{
			unloadChunk(minXPos, 0, z);
			createNewChunk(maxXPos, 0, z);
			loadNewChunk(maxXPos, 0, z);
		}

		minXPos = minXPos + 1;
		maxXPos = maxXPos + 1;
		m_needRebuild = true;
	}
	else if (m_pPlayer->getPosition().x < m_renderPosition.x + Voxel::CHUNK_SIZE)
	{
		
	}

	if (m_pPlayer->getPosition().y > m_renderPosition.y + Voxel::CHUNK_SIZE)
	{

	}
	else if (m_pPlayer->getPosition().y < m_renderPosition.y + Voxel::CHUNK_SIZE)
	{

	}

	if (m_pPlayer->getPosition().z > m_renderPosition.z + Voxel::CHUNK_SIZE)
	{

	}
	else if (m_pPlayer->getPosition().z < m_renderPosition.z + Voxel::CHUNK_SIZE)
	{

	}
}

void ChunkManager::createChunks()
{
	m_renderPosition = m_pPlayer->getPosition();

	minXPos = static_cast<int32_t>(m_pPlayer->getPosition().x) - m_pPlayer->getLoadRadius();
	minZPos = static_cast<int32_t>(m_pPlayer->getPosition().z) - m_pPlayer->getLoadRadius();
	maxXPos = static_cast<int32_t>(m_pPlayer->getPosition().x) + m_pPlayer->getLoadRadius();
	maxZPos = static_cast<int32_t>(m_pPlayer->getPosition().z) + m_pPlayer->getLoadRadius();

	for (int32_t x = minXPos; x < maxXPos; x++)
	{
		for (int32_t y = 0; y < 1; y++)
		{
			for (int32_t z = minZPos; z < maxZPos; z++)
			{
				createNewChunk(x, y, z);
			}
		}
	}
}
