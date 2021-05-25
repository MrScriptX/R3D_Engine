cmake_minimum_required(VERSION 3.15)

project(R3DEngine
    LANGUAGE CXX
    VERSION 1.2.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Engine)
add_subdirectory(ExampleCube)