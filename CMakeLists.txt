cmake_minimum_required(VERSION 3.7)

project(SDL_ENGINE)
set(CMAKE_BUILD_TYPE Debug)

set(SDL2_DIR C:/SDL)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

file( GLOB_RECURSE sources src/*.cpp src/*.h )
add_executable(out ${sources})
target_link_libraries(out ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
