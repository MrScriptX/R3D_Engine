#ifndef R3DENGINE_LIGHT_H_
#define R3DENGINE_LIGHT_H_

#include "Mesh.h"

class LightObject
{
  public:
	LightObject();
	~LightObject();

  private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
};

#endif // !R3DENGINE_LIGHT_H_