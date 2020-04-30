cmake_minimum_required(VERSION 3.0)

project(ex1 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror -g")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

#add_executable(map test_utilities.h map_example_test.c map.h map.c node.h node.c)

#link_directories(.)

add_executable(election map.h map.c node.c node.h test_utilities.h electionTestsMichal.c election.h election.c utilities.h utilities.c area.h area.c tribe.h tribe.c vote.h vote.c)

#target_link_libraries(election libmap_linux.a)