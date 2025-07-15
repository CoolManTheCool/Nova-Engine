cmake_minimum_required(VERSION 3.28)
project(demo_game)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

set(NOVA_ENGINE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(NOVA_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../app/engine)
set(LIBS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/app/libs)

add_executable(demo_game main.cpp)

target_include_directories(demo_game PRIVATE
    ${NOVA_ENGINE_INCLUDE_DIR}
    ${LIBS_INCLUDE_DIR}/imgui
    ${LIBS_INCLUDE_DIR}/imgui/backends
)

# Link the shared engine library
target_link_directories(demo_game PRIVATE ${NOVA_ENGINE_LIB_DIR})
target_link_libraries(demo_game PRIVATE nova_engine)

# Vulkan and GLFW might be needed for linking
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(demo_game PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(demo_game PRIVATE Vulkan::Vulkan glfw)

# Add compile flags
target_compile_options(demo_game PRIVATE -Wall -Wextra)
