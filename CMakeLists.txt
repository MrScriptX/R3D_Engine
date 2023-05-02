cmake_minimum_required(VERSION 3.10)

# set the project name
project(R3DEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF(DEFINED ENV{VULKAN_SDK})
    MESSAGE(STATUS "VULKAN PATH FOUND: $ENV{VULKAN_SDK}")
ELSE()
    MESSAGE(STATUS "VULKAN PATH NOT FOUND")
ENDIF()

file(GLOB_RECURSE ENGINE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Engine/*.cpp" "dependencies/imgui-1.87/*.cpp")

add_library(R3DEngine STATIC ${ENGINE_SOURCES})

IF(UNIX)
    target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3.7/lib/unix/libglfw3.a")
    target_link_libraries(R3DEngine PUBLIC "$ENV{VULKAN_SDK}/lib/libvulkan.so.1")
    target_link_libraries(R3DEngine PUBLIC "$ENV{VULKAN_SDK}/lib/libvulkan.so")
    
    if(CMAKE_DL_LIBS)
        target_link_libraries(R3DEngine PUBLIC ${CMAKE_DL_LIBS})
    endif()

    target_link_libraries(R3DEngine PUBLIC pthread -lX11 -lassimp)
ENDIF()

IF(WIN32)
    target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/assimp-5.0.1/lib/Debug/assimp-vc142-mtd.lib")
    target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3.7/lib/win/glfw3.lib")
    target_link_libraries(R3DEngine PUBLIC "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
ENDIF()

target_include_directories(R3DEngine PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/Engine")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/assimp-5.0.1/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/assimp-5.0.1/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3.7/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glm")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/stb_image")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/imgui-1.87")
target_include_directories(R3DEngine PUBLIC $ENV{VULKAN_SDK}/Include)

target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0/lib/Release/glew32.lib")


add_executable(ExampleCube ExampleCube/main.cpp)
target_link_libraries(ExampleCube R3DEngine)

add_executable(MovingLight MovingLight/main.cpp)
target_link_libraries(MovingLight R3DEngine)

add_executable(NormalMap NormalMap/main.cpp)
target_link_libraries(NormalMap R3DEngine)
