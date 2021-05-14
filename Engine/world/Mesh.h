#ifndef _MODEL_H
#define _MODEL_H

#include <memory>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"

#include "../graphics/Vertex.h"
#include "../renderer/Renderer.h"


class Mesh
{
public:
	Mesh(const int ID, std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Renderer> p_renderer);
	Mesh(const int ID, const std::string& obj_path, std::shared_ptr<Renderer> p_renderer);
	~Mesh();//two destructors | one for move contrutor and one for classic destruction

	void draw(const VkCommandBuffer& command_buffer, const int32_t frame);
	void loadModel();
	void bindMaterial(std::shared_ptr<Material> mat, VkBuffer& ubo, std::shared_ptr<Renderer> renderer);
	void Delete();

	void CreateBuffers(std::shared_ptr<Renderer> engine);
	void DestroyOldBuffers();
	void DestroyBuffers(const int32_t frame);

	void SetVertices(const std::vector<Vertex>& vertices);
	void SetIndices(const std::vector<uint32_t>& indices);

	std::vector<Vertex>& get_vertices();
	std::vector<uint32_t>& get_indices();
	Buffer& GetBuffer(const int32_t frame);
	Buffer& GetOldBuffer(const int32_t frame);
	std::shared_ptr<Material> getMaterial();
	const bool IsDestroyed();
	const bool IsCleaned();

private:
	int m_ID;
	std::string m_obj_path;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	std::array<Buffer, MAX_FRAMES_IN_FLIGHT> m_buffer;
	std::array<Buffer, MAX_FRAMES_IN_FLIGHT> m_old_buffer;

	std::shared_ptr<Material> p_material;
	std::shared_ptr<Renderer> mp_renderer;

	bool m_destroyed;
};

#endif//!_MODEL_H