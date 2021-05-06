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
	Mesh(std::shared_ptr<Renderer> p_renderer);
	Mesh(const std::string& obj_path, std::shared_ptr<Renderer> p_renderer);
	~Mesh();

	void draw(const VkCommandBuffer& command_buffer, const Pipeline& pipeline);
	void loadModel();
	void bindMaterial(std::shared_ptr<Material> mat, VkBuffer& ubo, std::shared_ptr<Renderer> renderer);
	void createBuffer(std::shared_ptr<Renderer> engine);

	void destroyMesh();

	std::vector<Vertex>& get_vertices();
	std::vector<uint32_t>& get_indices();
	Buffer& getBuffer();
	std::shared_ptr<Material> getMaterial();

private:
	const std::string m_obj_path;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	Buffer m_buffer;

	std::shared_ptr<Material> p_material;

	std::shared_ptr<Renderer> mp_renderer;
};

#endif//!_MODEL_H