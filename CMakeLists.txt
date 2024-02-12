set(PROJECT_NAME "Dynamic-Voxel-Craft")

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

project(${PROJECT_NAME})

find_package(SFML COMPONENTS graphics window system audio REQUIRED)
find_package(OpenAL REQUIRED)

set(SOURCE_FILES
    src/main.cpp
    src/game.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    include 
    ${SFML_INCLUDE_DIR} 
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio OpenAL)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)