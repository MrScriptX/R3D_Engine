#ifndef _MODEL_H
#define _MODEL_H

#include <memory>
#include <unordered_map>

#include "Vertex.h"
#include "renderer/Renderer.h"
#include "Material.h"


class Mesh
{
public:
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

/*Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(MODEL_PATH, aiProcessPreset_TargetRealtime_Quality);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		for (int t = 0; t < mesh->mNumVertices; t++)
		{
			Vertex vertex;
			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertex.texCoord.x = mesh->mTextureCoords[i]->x;
			vertex.texCoord.y = mesh->mTextureCoords[i]->y;

			vertices.push_back(vertex);
		}

		for (int x = 0; x < mesh->mFaces->mNumIndices; x++)
		{
			indices.push_back(mesh->mFaces->mIndices[x]);
		}
	}*/