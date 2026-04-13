cmake_minimum_required(VERSION 3.18)
project(as8 CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(../raylib-cpp raylib)

include(../assets/includeable.cmake)

add_executable(as8 as8.cpp skybox.cpp componentCounter.cpp)
target_link_libraries(as8 raylib raylib_cpp raygui)

find_package(TBB REQUIRED)
target_link_libraries(as8 PUBLIC TBB::tbb)

make_includeable(../assets/shaders/skybox.vs generated/skybox.vs)
make_includeable(../assets/shaders/skybox.fs generated/skybox.fs)
make_includeable(../assets/shaders/cubemap.vs generated/cubemap.vs)
make_includeable(../assets/shaders/cubemap.fs generated/cubemap.fs)

configure_file(../assets/models/penguin.glb models/penguin.glb COPYONLY)
configure_file(../assets/textures/skybox.png textures/skybox.png COPYONLY)
configure_file(../assets/textures/snow.jpg textures/snow.jpg COPYONLY)
