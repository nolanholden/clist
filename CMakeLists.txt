cmake_minimum_required(VERSION 2.8)

project(clist)
file(GLOB SOURCES src/*.c)
add_library(clist ${SOURCES})
target_include_directories(clist INTERFACE include/)
set_target_properties(clist PROPERTIES LINKER_LANGUAGE CXX)
include_directories(include/)

include(googletest.cmake)

SET(COVERAGE OFF CACHE BOOL "Coverage")

add_executable(cl_tests tests/clist_unittests.cc src/clist.c)

find_package (Threads)
target_link_libraries(cl_tests googletest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(cl_tests clist)

target_compile_options(cl_tests PRIVATE
  -g -Wall
  -Werror
  -Wextra
  -Wpedantic
  -Wconversion
  -Wswitch-default
  -Wswitch-enum
  -Wunreachable-code
  -Wwrite-strings
  -Wcast-align
  -Wshadow
  -Wundef)
  
if (COVERAGE)
    target_compile_options(cl_tests PRIVATE --coverage)
    target_link_libraries(cl_tests PRIVATE --coverage)
endif()

