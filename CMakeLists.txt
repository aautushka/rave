cmake_minimum_required(VERSION 2.8)

project(rave)
include(gtest.cmake)

add_compile_options(-std=gnu++1y -O3)

add_subdirectory(tests)
add_subdirectory(bench)
