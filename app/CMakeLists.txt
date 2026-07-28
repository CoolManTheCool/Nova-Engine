cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "nova-engine")
project(${PROJECT_NAME} VERSION 5.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# -------------------------
# ImGui (internal static lib)
# -------------------------
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

# Core ImGui headers are PUBLIC
# Backend headers are PRIVATE (build-only)
target_include_directories(imgui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui>
        $<INSTALL_INTERFACE:include/nova-engine/imgui>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends
)

target_compile_options(imgui PRIVATE -fPIC -Wall -Wextra)

# -------------------------
# Nova Engine library
# -------------------------
set(ENGINE_SOURCE_FILES
    engine/systems/gui_system.cpp
    engine/systems/system.cpp

    engine/data/rollingbuffer.cpp

    engine/graphics/vulkan/compute_pipeline.cpp
    engine/graphics/vulkan/pipeline_layout.cpp
    engine/graphics/vulkan/descriptors.cpp
    engine/graphics/vulkan/swap_chain.cpp
    engine/graphics/vulkan/renderer.cpp
    engine/graphics/vulkan/compute.cpp
    engine/graphics/vulkan/device.cpp
    engine/graphics/vulkan/buffer.cpp
    engine/graphics/graphics.cpp
    engine/graphics/window.cpp

    engine/objects/object.cpp

    engine/utility/config.cpp

    engine/modules/module_manager.cpp

    engine/engine.cpp
)

add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/nova-engine>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engine
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends
        ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        imgui
    PRIVATE
        Vulkan::Vulkan
        glfw
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# -------------------------
# Install headers
# -------------------------
# Engine public headers
install(DIRECTORY include/ DESTINATION include/nova-engine)

install(FILES
    libs/imgui/imgui.h
    libs/imgui/imconfig.h
    libs/imgui/imgui_internal.h
    libs/imgui/imstb_rectpack.h
    libs/imgui/imstb_textedit.h
    libs/imgui/imstb_truetype.h
    DESTINATION include/nova-engine/
)

# -------------------------
# Install libraries + export
# -------------------------
install(TARGETS ${PROJECT_NAME} imgui
    EXPORT nova-engine-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/nova-engine
)

install(EXPORT nova-engine-targets
    FILE nova-engine-targets.cmake
    NAMESPACE nova-engine::
    DESTINATION lib/cmake/nova-engine
)

# -------------------------
# Package configuration
# -------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engine-configVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nova-engine-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engine-config.cmake"
    INSTALL_DESTINATION lib/cmake/nova-engine
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engine-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nova-engine-configVersion.cmake"
    DESTINATION lib/cmake/nova-engine
)
