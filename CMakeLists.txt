set(PROJECT_NAME "Nova-Engine")

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)

project(${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

# Find and include GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find OpenAL | Disabled, as not used.
#find_package(OpenAL REQUIRED)

set(SOURCE_FILES
    ../src/graphics/systems/point_light_system.cpp
    ../src/graphics/systems/mesh_system.cpp
    src/graphics/descriptors.cpp
    src/graphics/swap_chain.cpp
    src/graphics/renderer.cpp
    src/graphics/pipeline.cpp
    src/graphics/window.cpp
    src/graphics/device.cpp
    src/graphics/buffer.cpp
    src/graphics/mesh.cpp
    src/objects/point_light_object.cpp
    src/objects/mesh_object.cpp
    src/objects/camera.cpp
    src/objects/object.cpp
    src/utility/resources.cpp
    src/utility/logger.cpp
    src/utility/util.cpp
    src/game/movement.cpp
    src/game/game.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    src/graphics
    src/graphics/systems
    src/objects
    src/utility
    src/game
    libs
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${OPENAL_LIBRARY} glfw Vulkan::Vulkan)

target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g -Wall -Wextra)