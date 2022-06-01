#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <vector>

struct Voxel
{
	std::vector<uint32_t> vertices;
	std::vector<uint16_t> indices;
};

#endif