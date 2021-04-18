#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <vector>

#include "renderer/Renderer.h"
#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	void loadTexture(const std::string& texture_path, std::shared_ptr<Renderer> renderer);
	std::shared_ptr<Texture> getTexture();
	void destroyTexture();

private:
	std::shared_ptr<Texture> m_texture;
};

#endif // !_MATERIAL_H