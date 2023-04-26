#ifndef VRED_RENDER_QUEUE_FAMILY_H
#define VRED_RENDER_QUEUE_FAMILY_H

#include <cstdint>

struct queue_family_indices
{
	int32_t graphic_family = -1;
	int32_t present_family = -1;

	[[nodiscard]] bool is_complete() const
	{
		return graphic_family >= 0 && present_family >= 0;
	}
};

#endif // !VRED_RENDER_QUEUE_FAMILY_H
