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
    src/main.cpp
    src/graphics/window.cpp
    src/graphics/pipeline.cpp
    src/graphics/device.cpp
    src/graphics/swap_chain.cpp
    src/game/game.cpp
    src/game/util.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    #headers
    src/game
    src/graphics
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${OPENAL_LIBRARY} glfw Vulkan::Vulkan)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O0)
