#ifndef _CHUNKSTORAGELOADER_H
#define _CHUNKSTORAGELOADER_H

#include <bitset>
#include "Chunk.h"
#include "Voxel.h"

class ChunkStorageLoader
{
public:
	ChunkStorageLoader(uint32_t x, uint32_t y, uint32_t z);
	~ChunkStorageLoader();

private:

	std::bitset<Voxel::CHUNK_SIZE * Voxel::CHUNK_SIZE * Voxel::CHUNK_SIZE> m_setVoxels;
};

#endif //!_CHUNKSTORAGELOADER_H