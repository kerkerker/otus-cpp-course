cmake_minimum_required(VERSION 3.10)

project(hello_world VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

add_subdirectory(source)
enable_testing()