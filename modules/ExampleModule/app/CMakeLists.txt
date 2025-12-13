cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME "ExampleModule")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} SHARED
    module.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "example_module")

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include # Our headers
    PRIVATE
        ${CMAKE_SOURCE_DIR}/app/include # Nova Engine public headers
        #${CMAKE_SOURCE_DIR}/app/engine # Engine internals (if needed)
)

target_link_libraries(${PROJECT_NAME} PRIVATE nova_engine)

install(DIRECTORY include/
    DESTINATION include/nova/modules/${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

target_compile_definitions(${PROJECT_NAME} PRIVATE NOVA_MODULE_BUILD)