cmake_minimum_required(VERSION 3.24)
project(taskgen)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
link_libraries(pthread)
add_executable(taskgen main.cpp)
