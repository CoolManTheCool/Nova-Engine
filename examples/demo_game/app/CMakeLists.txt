cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "demo_game")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

set(NOVA_ENGINE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(NOVA_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../app/engine)
set(LIBS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/app/libs)

# Gather all .cpp and .hpp files in game/ recursively
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/game/*.hpp
)

add_executable(${PROJECT_NAME} main.cpp ${GAME_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${NOVA_ENGINE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/game
)

# Link the shared engine library
target_link_directories(${PROJECT_NAME} PRIVATE ${NOVA_ENGINE_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE nova_engine)

target_link_libraries(demo_game PRIVATE ExampleModule)

# Add compile flags
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
