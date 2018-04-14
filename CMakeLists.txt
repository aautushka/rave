cmake_minimum_required(VERSION 2.8)

project(rave)
include(gtest.cmake)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_compile_options(-std=gnu++1y -O3)

set (BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)
add_subdirectory(tests)
add_subdirectory(bench)
