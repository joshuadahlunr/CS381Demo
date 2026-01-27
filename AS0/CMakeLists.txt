cmake_minimum_required(VERSION 3.18)
project(as0 CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../raylib-cpp raylib)

add_executable(as0 as0.cpp)
target_link_libraries(as0 raylib raylib_cpp)