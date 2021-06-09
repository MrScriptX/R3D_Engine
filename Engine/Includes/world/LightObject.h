#ifndef R3DENGINE_LIGHT_H_
#define R3DENGINE_LIGHT_H_

#include "../graphics/Vertex.h"

class LightObject
{
  public:
	LightObject();
	~LightObject();

	Light& GetTransform();

	void SetTransform(Light& transform);

  private:
	Light m_transform;
};

#endif // !R3DENGINE_LIGHT_H_