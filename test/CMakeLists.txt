set(CMAKE_CXX_STANDARD 23)

include_directories($ENV{HOME}/Projects)

add_executable(test_circular_buffer test_circular_buffer.cpp)
target_link_libraries(test_circular_buffer circular_buffer)