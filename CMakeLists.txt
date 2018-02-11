cmake_minimum_required(VERSION 2.8)

add_library(clist src/clist.c)
set_target_properties(clist PROPERTIES LINKER_LANGUAGE C)
#target_include_directories(clist INTERFACE include/)

include(googletest.cmake)

SET(COVERAGE OFF CACHE BOOL "Coverage")

add_executable(tests tests/clist_unittests.cc)

find_package (Threads)
target_link_libraries(tests googletest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(tests clist)

target_compile_options(tests PRIVATE
  -g -Wall
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
    target_compile_options(tests PRIVATE --coverage)
    target_link_libraries(tests PRIVATE --coverage)
endif()

