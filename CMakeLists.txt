set(PROJECT_NAME "Dynamic-Voxel-Craft")

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

# Find SFML
find_package(SFML COMPONENTS graphics window system audio REQUIRED)
find_package(OpenAL REQUIRED)

# Add ImGui source files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
set(IMGUI_SFML_DIR ${CMAKE_SOURCE_DIR}/include/imgui-sfml)
set(IMGUI_SFML_FILES
    ${IMGUI_SFML_DIR}/imgui-SFML.cpp
    ${IMGUI_SFML_DIR}/imgui-SFML.h
)

set(SOURCE_FILES
    src/main.cpp
    src/game.cpp
    src/object.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${IMGUI_SFML_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    include 
    ${SFML_INCLUDE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_SFML_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio OpenAL)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)