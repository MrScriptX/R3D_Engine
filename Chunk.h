#ifndef _CHUNK_H
#define _CHUNK_H

#include <array>
#include <bitset>

#include "Voxel.h"
#include "Renderer.h"

class Chunk
{
public:
	Chunk(std::shared_ptr<Renderer> pRenderer);
	~Chunk();

	void setup();
	void load();

	void createMesh();
	void destroyMesh();

	size_t getSizeIndices();
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	Buffer& getBuffers();

	void setPosition(float x, float y, float z);
	
private:

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	std::bitset<Voxel::CHUNK_SIZE * Voxel::CHUNK_SIZE * Voxel::CHUNK_SIZE> m_activeVox;

	std::shared_ptr<Renderer> m_pRenderer;
	std::unique_ptr<Buffer> m_pBuffer;
	Mesh m_mesh;
};

#endif // !_CHUNK_H