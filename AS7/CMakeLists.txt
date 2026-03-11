cmake_minimum_required(VERSION 3.18)
project(as7 CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../raylib-cpp raylib)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ../asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

include(../assets/includeable.cmake)

add_executable(as7 as7.cpp skybox.cpp)
target_link_libraries(as7 raylib raylib_cpp raygui raylib::buffered asio)

make_includeable(../assets/shaders/skybox.vs generated/skybox.vs)
make_includeable(../assets/shaders/skybox.fs generated/skybox.fs)
make_includeable(../assets/shaders/cubemap.vs generated/cubemap.vs)
make_includeable(../assets/shaders/cubemap.fs generated/cubemap.fs)

configure_file(../assets/models/penguin.glb models/penguin.glb COPYONLY)
configure_file(../assets/textures/skybox.png textures/skybox.png COPYONLY)
configure_file(../assets/textures/snow.jpg textures/snow.jpg COPYONLY)
