cmake_minimum_required(VERSION 3.28)
project(demo-game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin)

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_executable(demo-game main.cpp ${GAME_SOURCES})

target_include_directories(demo-game PRIVATE ../app/include)
target_include_directories(demo-game PRIVATE ../app/libs)
target_link_libraries(demo-game PRIVATE nova-engine)

target_compile_options(demo-game PRIVATE -Wall -Wextra)
