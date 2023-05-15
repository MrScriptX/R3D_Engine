#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <vector>

#include "../renderer/Renderer.h"

#include "Texture.h"

class Material
{
  public:
	Material(const std::string& shader, std::shared_ptr<Renderer> p_renderer);
	~Material();

	void setColor(glm::vec3 color);
	void clearColor();

	void LoadTexture(const std::string& texture_path);
	void DestroyTexture();

	std::shared_ptr<glm::vec3> getColor();
	std::shared_ptr<Texture> getTexture();
	VkDescriptorSet& getDescriptorSet();

	void set_pipeline(const std::string& pipeline);
	const std::string& pipeline() const;

  private:
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<glm::vec3> m_color;

	std::string m_pipeline;
	VkDescriptorSet m_descriptor_set;
	std::shared_ptr<Renderer> mp_renderer;
};

#endif // !_MATERIAL_H
