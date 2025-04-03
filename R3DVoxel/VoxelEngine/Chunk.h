#ifndef R3DENGINE_CHUNK_H_
#define R3DENGINE_CHUNK_H_

#include <bitset>
#include <Engine.h>

#include "Voxel.h"
#include "Cube.h"

struct ChunkKey
{
	int32_t x;
	int32_t y;
	int32_t z;
};

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

class Chunk
{
public:
	Chunk(const int32_t posx, const int32_t posy, const int32_t posz);

	void BuildChunk(const std::map<ChunkKey, std::unique_ptr<Chunk>>& chunk_map, std::shared_ptr<GameObject> world, std::shared_ptr<Material> mat);
	void UpdateChunk(const std::map<ChunkKey, std::unique_ptr<Chunk>>& chunk_map, GameObject& world);
	void DeleteChunk(std::shared_ptr<GameObject> world);

	std::optional<Geometry> compute_mesh(const std::map<ChunkKey, std::unique_ptr<Chunk>>& chunk_map);
	void render_mesh(const Geometry& mesh, GameObject& world, std::shared_ptr<Material> mat);



	void SetVoxel(const uint32_t x, const uint32_t y, const uint32_t z);
	void SetBlockType(const uint32_t x, const uint32_t y, const uint32_t z, const TBlock type);

	bool GetVoxel(const uint32_t x, const uint32_t y, const uint32_t z) const;

	glm::vec3 GetPosition() const;
	int32_t mesh_id() const;

private:
	void CreateCube(Geometry& mesh, uint32_t x, uint32_t y, uint32_t z);

	int32_t m_mesh_id;
	glm::vec3 m_position;

	std::bitset<Voxel::CHUNK_SIZE_CUBE> m_active_voxel;
	std::array <Voxel::Faces, Voxel::CHUNK_SIZE_CUBE > m_visible_voxel;
	std::array<TBlock, Voxel::CHUNK_SIZE_CUBE> m_blocktypes;
};

#endif //!R3DENGINE_CHUNK_H_