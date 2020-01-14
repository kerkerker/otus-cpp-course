cmake_minimum_required(VERSION 3.10)

project(hello_world VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

configure_file(source/version.h.in version.h)

add_executable(hello_world_cli source/main.cpp)
add_library(hello_world source/lib.cpp)

target_include_directories(hello_world PUBLIC
        "${PROJECT_BINARY_DIR}"
        )

target_link_libraries(hello_world_cli PUBLIC hello_world)

