set(PROJECT_NAME "Dynamic-Voxel-Craft")

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

# Find and include GLFW
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Find OpenAL
find_package(OpenAL REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    src/window/window.cpp
    src/app/app.cpp
    src/pipeline/pipeline.cpp
    src/device/device.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    #headers
    src/app
    src/window
    src/pipeline
    src/device
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} ${OPENAL_LIBRARY} glfw Vulkan::Vulkan)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
