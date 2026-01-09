cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "nova-engine")
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

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
# Engine Source Files
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

    engine/modules/module_manager.cpp
    
    engine/objects/mesh.cpp
    engine/objects/point_light_object.cpp
    engine/objects/camera.cpp
    engine/objects/object.cpp

    engine/utility/resources.cpp
    engine/utility/util.cpp

    engine/engine.cpp
)

add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCE_FILES})

# ----------------------------------------------------
# Include directories
# ----------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/nova-engine>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/data
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/utility
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/systems
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/graphics
        ${CMAKE_CURRENT_SOURCE_DIR}/engine/modules
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        ${Vulkan_INCLUDE_DIRS}
)

# ----------------------------------------------------
# Link libraries
# ----------------------------------------------------
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Vulkan::Vulkan
        glfw
    PRIVATE
        imgui
)

# Compile options
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# ----------------------------------------------------
# Install rules
# ----------------------------------------------------
install(DIRECTORY include/ DESTINATION include/nova-engine)

install(TARGETS ${PROJECT_NAME}
    EXPORT nova-engineTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/nova-engine
)

# Export CMake package
install(EXPORT nova-engineTargets
    FILE nova-engineTargets.cmake
    NAMESPACE nova-engine::
    DESTINATION lib/cmake/nova-engine
)

include(CMakePackageConfigHelpers)

# Generate version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engineConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nova-engineConfig.cmake.in" # input template
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engineConfig.cmake"            # output
    INSTALL_DESTINATION "lib/cmake/nova-engine"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Install package config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engineConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engineConfigVersion.cmake"
    DESTINATION lib/cmake/nova-engine
)
