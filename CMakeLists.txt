set(PROJECT_NAME "Nova-Engine")

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)

project(${PROJECT_NAME})

# Find and include GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find OpenAL
find_package(OpenAL REQUIRED)

set(SOURCE_FILES
    src/graphics/systems/point_light_system.cpp
    src/graphics/systems/render_system.cpp
    src/graphics/descriptors.cpp
    src/graphics/swap_chain.cpp
    src/utility/resources.cpp
    src/graphics/renderer.cpp
    src/graphics/pipeline.cpp
    src/graphics/window.cpp
    src/graphics/device.cpp
    src/graphics/camera.cpp
    src/graphics/object.cpp
    src/graphics/buffer.cpp
    src/graphics/model.cpp
    src/utility/logger.cpp
    src/game/movement.cpp
    src/utility/util.cpp
    src/game/game.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    #headers
    src/graphics/systems
    src/graphics
    src/utility
    src/game
    libs
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${OPENAL_LIBRARY} glfw Vulkan::Vulkan)

target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra)

#experimental
#target_compile_options(${PROJECT_NAME} PRIVATE -flto -ffast-math -march=native -Ofast)
#No differnce 4/26/24 Only have a triangle in code and starting tutorial 07
