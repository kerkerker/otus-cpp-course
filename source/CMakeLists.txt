cmake_minimum_required(VERSION 3.10)

if (NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 1)
    message(STATUS "TRAVIS_BUILD_NUMBER must be set! Using default ($ENV{TRAVIS_BUILD_NUMBER}) instead...")
endif ()

project(ip_addresses VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_executable(ip_addresses_cli main.cpp)
add_library(ip_addresses lib.cpp)
add_executable(test_ip test_ip.cpp)

set_target_properties(test_ip PROPERTIES
        INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
        )

target_link_libraries(ip_addresses_cli PUBLIC ip_addresses)
target_link_libraries(test_ip GTest::GTest GTest::Main ip_addresses)

install(TARGETS ip_addresses_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT ker@scryptmail.com)

include(CPack)

enable_testing()

add_test(test_ip test_ip)