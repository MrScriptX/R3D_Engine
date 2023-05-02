#ifndef VRED_RENDERER_PARAMETERS_H
#define VRED_RENDERER_PARAMETERS_H

namespace vred::renderer::parameters
{
#ifdef NDEBUG
	constexpr bool validation_layer_enable = false;
#else
	constexpr bool validation_layer_enable = true;
#endif
}

#endif // !VRED_RENDERER_PARAMETERS_H
