#ifndef VRED_RENDERER_PIPELINE_H
#define VRED_RENDERER_PIPELINE_H

#include <vulkan/vulkan.h>

namespace vred::renderer
{
	struct ipipeline
	{
		VkPipeline handle;
		VkPipelineLayout layout;
	};
};

#endif // !VRED_RENDERER_PIPELINE_H
