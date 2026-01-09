cmake_minimum_required(VERSION 3.28)

project(demo-game VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

# ----------------------------------------------------
# Find installed Nova Engine
# ----------------------------------------------------
find_package(nova-engine REQUIRED)

# ----------------------------------------------------
# Gather all game sources
# ----------------------------------------------------
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/*.hpp
)

# ----------------------------------------------------
# Build executable
# ----------------------------------------------------
add_executable(${PROJECT_NAME} main.cpp ${GAME_SOURCES})

# Include game sources
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/game
)

# Link against Nova Engine
target_link_libraries(${PROJECT_NAME} PRIVATE nova-engine::nova-engine)

# Link any additional modules
target_link_libraries(${PROJECT_NAME} PRIVATE ExampleModule)

# Compile flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
