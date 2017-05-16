cmake_minimum_required(VERSION 3.0)

project(rave)

add_compile_options(-std=gnu++1y -O3)

add_subdirectory(tests)
add_subdirectory(bench)
