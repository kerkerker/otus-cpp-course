cmake_minimum_required(VERSION 3.10)

if (NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 1)
    message(STATUS "TRAVIS_BUILD_NUMBER must be set! Using default ($ENV{TRAVIS_BUILD_NUMBER}) instead...")
endif ()

project(bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_executable(bulkmt main.cpp bulk.cpp)
add_executable(test_bulkmt test.cpp bulk.cpp)

set_target_properties(test_bulkmt PROPERTIES
        INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
        )

target_link_libraries(bulkmt ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test_bulkmt GTest::GTest GTest::Main ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT ker@scryptmail.com)

include(CPack)

enable_testing()

add_test(test_bulkmt test_bulkmt)