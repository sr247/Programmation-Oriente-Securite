cmake_minimum_required(VERSION 3.8)
project(parser_mp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.h)
add_executable(parser_mp ${SOURCE_FILES} Image.h)