cmake_minimum_required(VERSION 3.10)

# set the project name
project(R3DEngine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF(DEFINED ENV{VULKAN_SDK})
    MESSAGE(STATUS "VULKAN PATH FOUND: $ENV{VULKAN_SDK}")
ELSE()
    MESSAGE(STATUS "VULKAN PATH NOT FOUND")
ENDIF()

file(GLOB_RECURSE ENGINE_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "Engine/*.cpp")

# compile Engine
add_library(R3DEngine STATIC ${ENGINE_SOURCES})

# compile exe
add_executable(ExampleCube ExampleCube/main.cpp)

target_include_directories(R3DEngine PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/Engine/Includes")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/assimp-5.0.1/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3.4/include")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glm")
target_include_directories(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/stb_image")
target_include_directories(R3DEngine PUBLIC $ENV{VULKAN_SDK}/Include)

target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/assimp-5.0.1/lib/Debug/assimp-vc142-mtd.lib")
target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glfw-3.3.4/lib/glfw3.lib")
target_link_libraries(R3DEngine PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/glew-2.1.0/lib/Release/x64/glew32.lib")
target_link_libraries(R3DEngine PUBLIC "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
target_link_libraries(ExampleCube R3DEngine)