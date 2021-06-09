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

	Light& GetTransform();
	const TLIGHT GetType();

	void SetTransform(Light& transform);

  private:
	TLIGHT m_type;
	Light m_transform;
};

#endif // !R3DENGINE_LIGHT_H_