#ifndef _CHUNKMANAGER_H
#define _CHUNKMANAGER_H

#include <vector>
#include <iostream>
#include <iterator>
#include <memory>
#include <map>

#include "ChunkStorageLoader.h"
#include "Renderer.h"
#include "Player.h"



struct ChunkKey
{
	int32_t x;
	int32_t y;
	int32_t z;
};

//operator for ChunkKey
inline bool const operator==(const ChunkKey& l, const ChunkKey& r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z;
}

inline bool const operator<(const ChunkKey& l, const ChunkKey& r)
{
	if (l.x < r.x)  return true;
	if (l.x > r.x)  return false;

	if (l.y < r.y)  return true;
	if (l.y > r.y)  return false;

	if (l.z < r.z)  return true;
	if (l.z > r.z)  return false;

	return false;
}

class ChunkManager
{
public:
	ChunkManager(std::shared_ptr<Renderer> pRenderer, std::shared_ptr<Player> pPlayer);
	~ChunkManager();

	void update();
	void createChunks();
	void loadChunks();
	void renderChunks(VkCommandBuffer& commandBuffer, Pipeline& pipeline);


	void createNewChunk(int32_t x, int32_t y, int32_t z);
	void unloadChunk(int32_t x, int32_t y, int32_t z);
	void loadNewChunk(int32_t x, int32_t y, int32_t z);

	Buffer& getBuffers(int32_t x, int32_t y, int32_t z);
	size_t getIndicesSize(int32_t x, int32_t y, int32_t z);

	bool needRebuild();
	void setRebuild(bool isRebuilt);
private:

	bool m_needRebuild;

	int32_t minXPos;
	int32_t minZPos;
	int32_t maxXPos;
	int32_t maxZPos;

	glm::vec3 m_renderPosition;

	std::shared_ptr<Renderer> m_pRenderer;
	std::shared_ptr<Player> m_pPlayer;
	std::map<ChunkKey, std::unique_ptr<Chunk>> m_chunkMap;
};

#endif // !_CHUNKMANAGER_H