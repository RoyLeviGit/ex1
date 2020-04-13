cmake_minimum_required(VERSION 3.0)

project(ex1 VERSION 0.1.0 LANGUAGES C)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

link_directories(.)

add_executable(ex1 test_utilities.h map_example_test.c map.h map.c)

target_link_libraries(ex1 libmap.a)