cmake_minimum_required(VERSION 3.15)
project(Test)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLEW REQUIRED glew)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_subdirectory(imgui-master)
add_subdirectory(src)
