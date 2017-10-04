cmake_minimum_required(VERSION 3.8)
project(parser_mp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Image.h)
add_executable(parser_mp ${SOURCE_FILES} Image.h)