F:\Programs\VulkanSDK\1.2.170.0\Bin32\glslangValidator.exe -V shader.vert
F:\Programs\VulkanSDK\1.2.170.0\Bin32\glslangValidator.exe -V texture_shader.frag
F:\Programs\VulkanSDK\1.2.170.0\Bin32\glslangValidator.exe -V no_texture_shader.frag
pause
F:\Programs\VulkanSDK\1.2.170.0\Bin32\glslc.exe shader.vert -o vert.spv
F:\Programs\VulkanSDK\1.2.170.0\Bin32\glslc.exe texture_shader.frag -o texture_shader.spv
F:\Programs\VulkanSDK\1.2.170.0\Bin32\glslc.exe no_texture_shader.frag -o no_texture_shader.spv