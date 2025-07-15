set(PROJECT_NAME "Nova-Engine")

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)

project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Engine source files (no main.cpp)
set(ENGINE_SOURCE_FILES
    engine/systems/physics_system.cpp
    engine/systems/mesh_system.cpp
    engine/systems/gui_system.cpp
    engine/systems/console.cpp
    engine/systems/editor.cpp
    engine/data/rollingbuffer.cpp
    engine/graphics/descriptors.cpp
    engine/graphics/swap_chain.cpp
    engine/graphics/renderer.cpp
    engine/graphics/pipeline.cpp
    engine/graphics/window.cpp
    engine/graphics/device.cpp
    engine/graphics/buffer.cpp
    engine/graphics/mesh.cpp
    engine/objects/point_light_object.cpp
    engine/objects/mesh_object.cpp
    engine/objects/camera.cpp
    engine/objects/object.cpp
    engine/utility/resources.cpp
    engine/utility/util.cpp
    engine/engine.cpp
)

set(IMGUI_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends
)

target_compile_options(imgui PRIVATE -fPIC)

add_library(nova_engine SHARED ${ENGINE_SOURCE_FILES})

target_include_directories(nova_engine PUBLIC
    engine/systems/
    engine/objects/
    engine/data/
    engine/utility/
    engine/graphics/
    engine/
    libs/
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(nova_engine PRIVATE ${Vulkan_LIBRARIES} glfw Vulkan::Vulkan imgui)

target_compile_options(nova_engine PRIVATE -Wall -Wextra)