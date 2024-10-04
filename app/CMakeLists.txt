set(PROJECT_NAME "Nova-Engine")

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)

project(${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find OpenAL | Disabled, as not used.
#find_package(OpenAL REQUIRED)

set(SOURCE_FILES
    engine/systems/point_light_system.cpp
    engine/systems/mesh_system.cpp
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
    engine/objects/editor.cpp
    engine/objects/camera.cpp
    engine/objects/object.cpp
    engine/utility/resources.cpp
    engine/utility/logger.cpp
    engine/utility/util.cpp
    engine/utility/gui_system.cpp
    engine/engine.cpp
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
    engine/systems/
    engine/objects/
    engine/utility/
    engine/graphics/
    engine/
    src/game/
    libs/

    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${OPENAL_LIBRARY} glfw Vulkan::Vulkan imgui)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)