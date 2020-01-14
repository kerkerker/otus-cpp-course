cmake_minimum_required(VERSION 3.10)

project(hello_world VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

configure_file(source/version.h.in version.h)

add_executable(hello_world_cli source/main.cpp)
add_library(hello_world source/lib.cpp)
add_executable(test_version source/test_version.cpp)

target_include_directories(hello_world PUBLIC
        "${PROJECT_BINARY_DIR}"
        )

set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_link_libraries(hello_world_cli PUBLIC hello_world)

target_link_libraries(test_version ${Boost_LIBRARIES} hello_world)
