set(PROJECT_NAME "Nova-Engine")

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)

project(${PROJECT_NAME})

# Find and include GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find OpenAL | Disabled, as not used.
#find_package(OpenAL REQUIRED)

set(SOURCE_FILES
    src/graphics/systems/point_light_system.cpp
    src/graphics/systems/mesh_system.cpp
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

set(IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/libs/imgui
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends
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

target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${OPENAL_LIBRARY} glfw Vulkan::Vulkan imgui)

target_compile_options(${PROJECT_NAME} PRIVATE -O2 -g -Wall -Wextra)