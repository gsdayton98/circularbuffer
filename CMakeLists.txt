cmake_minimum_required(VERSION 3.25)
project(circular_buffer)

set(CMAKE_CXX_STANDARD 23)

add_library(circular_buffer SHARED circular_buffer.hpp circular_buffer.cpp)

add_subdirectory(test)