cmake_minimum_required(VERSION 3.7)

project(GLFW_GL_ENGINE)
set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

file( GLOB_RECURSE sources src/*.cpp src/*.h )
add_executable(out ${sources})
target_link_libraries(out ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
