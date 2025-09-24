cmake_minimum_required(VERSION 3.28)
set(PROJECT_NAME "Nova-Engine")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_BUILD_TYPE Debug)

# ----------------------------------------------------
# Dependencies
# ----------------------------------------------------

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# ----------------------------------------------------
# IMGUI Library
# ----------------------------------------------------

set(IMGUI_SRC
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    libs/imgui
    libs/imgui/backends
)
target_compile_options(imgui PRIVATE -fPIC)

# ----------------------------------------------------
# Engine Source Files (explicit)
# ----------------------------------------------------

set(ENGINE_SOURCE_FILES
    engine/systems/mesh_system.cpp
    engine/systems/gui_system.cpp
    #engine/systems/console.cpp
    #engine/systems/editor.cpp

    engine/data/rollingbuffer.cpp

    engine/graphics/descriptors.cpp
    engine/graphics/swap_chain.cpp
    engine/graphics/renderer.cpp
    engine/graphics/graphics.cpp
    engine/graphics/pipeline.cpp
    engine/graphics/window.cpp
    engine/graphics/device.cpp
    engine/graphics/buffer.cpp
    
    engine/objects/mesh.cpp
    engine/objects/point_light_object.cpp
    engine/objects/camera.cpp
    engine/objects/object.cpp

    engine/utility/resources.cpp
    engine/utility/util.cpp

    engine/engine.cpp
)

add_library(nova_engine SHARED ${ENGINE_SOURCE_FILES})

target_include_directories(nova_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/objects
    PRIVATE
        engine/data
        engine/utility
        engine/systems
        engine/graphics
        engine/graphics/mesh
        engine
        libs
        ${CMAKE_CURRENT_SOURCE_DIR}/include/objects
        ${Vulkan_INCLUDE_DIRS}
)

# ----------------------------------------------------
# Link libraries
# ----------------------------------------------------
target_link_libraries(nova_engine
    PRIVATE
        ${Vulkan_LIBRARIES}
        glfw
        Vulkan::Vulkan
        imgui
)

target_compile_options(nova_engine PRIVATE -Wall -Wextra)

# ----------------------------------------------------
# Install Rules for SDK Distribution
# ----------------------------------------------------

install(DIRECTORY include/ DESTINATION include/nova)

install(TARGETS nova_engine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
