#ifndef R3DENGINE_LIGHT_H_
#define R3DENGINE_LIGHT_H_

#include "../graphics/Vertex.h"

enum class TLIGHT
{
	DIRECTIONAL = 0,
	SPOT = 1,
	POINT = 2
};

class LightObject
{
  public:
	LightObject(TLIGHT type);
	~LightObject();

	void SetPosition(glm::vec3 position);
	void SetDirection(glm::vec3 direction);
	void SetColor(glm::vec3 color);
	void SetAmbientStrength(const float ambient);
	void SetDiffuseStrength(const float diffuse);
	void SetSpecularStrength(const float specular);
	void SetProperties(const LightProperties properties);

	const LightProperties& GetProperties();
	const TLIGHT GetType();

  private:
	TLIGHT m_type;
	LightProperties m_properties;
};

#endif // !R3DENGINE_LIGHT_H_