cmake_minimum_required(VERSION 3.20)

project(aoc-2021)

set(BOOST_ROOT /usr/include/boost169)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(-std=c++2a -ggdb -Wall)  # -Og makes it faster, but hard to debug

add_executable(aoc-2021 main.cpp runner.cpp week1.cpp week2.cpp)
