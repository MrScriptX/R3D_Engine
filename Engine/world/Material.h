#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <vector>

#include "../renderer/Renderer.h"

#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	void setColor(glm::vec3 color);
	void clearColor();

	void loadTexture(const std::string& texture_path, std::shared_ptr<Renderer> renderer);
	void destroyTexture();

	std::shared_ptr<glm::vec3> getColor();
	std::shared_ptr<Texture> getTexture();
	VkDescriptorSet& getDescriptorSet();
private:
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<glm::vec3> m_color;

	VkDescriptorSet m_descriptor_set;
};

#endif // !_MATERIAL_H