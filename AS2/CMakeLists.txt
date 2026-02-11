cmake_minimum_required(VERSION 3.18)
project(as2 CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../raylib-cpp raylib)

add_executable(as2 as2.cpp)
target_link_libraries(as2 raylib raylib_cpp raygui)

configure_file(../assets/models/penguin.glb models/penguin.glb COPYONLY)
