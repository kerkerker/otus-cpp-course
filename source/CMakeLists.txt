cmake_minimum_required(VERSION 3.10)

if (NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(ENV{TRAVIS_BUILD_NUMBER} 1)
    message(STATUS "TRAVIS_BUILD_NUMBER must be set! Using default ($ENV{TRAVIS_BUILD_NUMBER}) instead...")
endif ()

project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost)
find_package(Threads)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_library(async SHARED async.cpp AsyncBulkManager.cpp bulk.cpp)
add_executable(bulk_server main.cpp BulkServer.cpp)
add_executable(test_bulk test.cpp bulk.cpp)

set_target_properties(async PROPERTIES PUBLIC_HEADER async.h)

set_target_properties(test_bulk PROPERTIES
        INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
        )

target_include_directories(bulk_server PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(bulk_server async ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
target_link_libraries(test_bulk GTest::GTest GTest::Main)

install(TARGETS async LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)
install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT ker@scryptmail.com)

include(CPack)

enable_testing()

add_test(test_bulk test_bulk)