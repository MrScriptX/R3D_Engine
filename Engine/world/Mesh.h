#ifndef R3DENGINE_MODEL_H_
#define R3DENGINE_MODEL_H_

#include <memory>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "LightObject.h"
#include "Material.h"

#include "../graphics/Vertex.h"
#include "../renderer/Renderer.h"
#include "../renderer/vpipeline.h"

class Mesh
{
  public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Renderer> p_renderer);
	Mesh(const std::string& obj_path, std::shared_ptr<Renderer> p_renderer);
	~Mesh(); // two destructors | one for move contrutor and one for classic destruction

	void draw(const VkCommandBuffer& command_buffer, VkDescriptorSet& descriptorset, const int32_t frame, const std::unordered_map<std::string, vred::renderer::ipipeline>& pipelines);
	void loadModel();
	void bindMaterial(std::shared_ptr<Material> mat, VkBuffer& ubo, std::shared_ptr<Renderer> renderer);

	void CreateBuffers(std::shared_ptr<Renderer> engine);
	void UpdateBuffers(const int32_t frame);
	void DestroyBuffers(const int32_t frame);

	void SetVertices(const std::vector<Vertex>& vertices);
	void SetIndices(const std::vector<uint32_t>& indices);

	std::vector<Vertex>& get_vertices();
	std::vector<uint32_t>& get_indices();
	Buffer& GetBuffer(const int32_t frame);
	std::shared_ptr<Material> getMaterial();
	const bool IsCleaned();
	const bool IsUpdated();

  private:
	std::string m_obj_path;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	std::array<Buffer, MAX_FRAMES_IN_FLIGHT> m_buffer;

	std::shared_ptr<Material> p_material;
	std::shared_ptr<Renderer> mp_renderer;

	std::bitset<MAX_FRAMES_IN_FLIGHT> m_to_update;
};

#endif // !R3DENGINE_MODEL_H_
