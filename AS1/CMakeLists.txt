cmake_minimum_required(VERSION 3.18)
project(as1 CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../raylib-cpp raylib)

add_executable(as1 as1.cpp raygui.cpp)
target_link_libraries(as1 raylib raylib_cpp raygui)

configure_file(../assets/audio/ping.wav audio/ping.wav COPYONLY)
configure_file(../assets/audio/price-of-freedom.mp3 audio/price-of-freedom.mp3 COPYONLY)