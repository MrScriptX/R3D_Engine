%VULKAN_SDK%\Bin\glslc.exe shader.vert -o ../vert.spv
%VULKAN_SDK%\Bin\glslc.exe normalmap.vert -o ../normalmap.spv
%VULKAN_SDK%\Bin\glslc.exe texture_shader.frag -o ../texture_shader.spv
%VULKAN_SDK%\Bin\glslc.exe no_texture_shader.frag -o ../no_texture_shader.spv
%VULKAN_SDK%\Bin\glslc.exe light_source.frag -o ../light_source.spv
%VULKAN_SDK%\Bin\glslc.exe default.frag -o ../frag.spv