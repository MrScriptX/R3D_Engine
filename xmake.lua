add_rules("mode.debug", "mode.release")

set_languages("cxx20")

target("Engine")
    set_kind("static")

    if is_os("windows") then
        add_links("kernel32", "gdi32", "user32", "shell32", "msvcrt")
        add_linkdirs("dependencies/glfw-3.3.7/lib/win")
    end

    add_includedirs("$(env VULKAN_SDK)/Include", { public = true })
    add_links("$(env VULKAN_SDK)/Lib/vulkan-1")

    add_includedirs("dependencies/glfw-3.3.7/include", { public = true })
    add_links("glfw3")

    add_includedirs("dependencies/glew-2.1.0/include", { public = true })
    add_links("dependencies/glew-2.1.0/lib/Release/glew32")

    add_includedirs("dependencies/assimp-5.0.1/include", { public = true })
    add_links("dependencies/assimp-5.0.1/lib/Debug/assimp-vc142-mtd")

    add_includedirs("dependencies/glm", { public = true })
    add_includedirs("dependencies/stb_image", { public = true })
    add_includedirs("dependencies/imgui-1.87", { public = true })
    add_includedirs("Engine", { public = true })

    add_files("Engine/**.cpp", "dependencies/imgui-1.87/**.cpp")

    after_build(function(target)
        import("core.project.config")
        os.cp("$(projectdir)/assets", "$(buildir)/$(os)/$(arch)/assets")
        os.cp("$(projectdir)/dependencies/assimp-5.0.1/lib/Debug/assimp-vc142-mtd.dll", "$(buildir)/$(os)/$(arch)/$(mode)")
    end) 
    

target("ExampleCube")
    set_kind("binary")
    add_deps("Engine")
    add_files("ExampleCube/*.cpp")    

target("MovingLight")
    set_kind("binary")
    add_deps("Engine")
    add_files("MovingLight/**.cpp")

target("NormalMap")
    set_kind("binary")
    add_deps("Engine")
    add_files("NormalMap/**.cpp")