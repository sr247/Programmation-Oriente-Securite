cmake_minimum_required(VERSION 3.5)
project(parser_mp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

set(SOURCE_FILES main.cpp Image.h)
add_executable(parser_mp ${SOURCE_FILES})
