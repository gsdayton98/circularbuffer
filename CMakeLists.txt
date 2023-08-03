cmake_minimum_required(VERSION 3.25)
project(circularbuffer)

set(CMAKE_CXX_STANDARD 23)

add_library(circularbuffer SHARED circularbuffer.hpp circularbuffer.cpp)

add_subdirectory(test)