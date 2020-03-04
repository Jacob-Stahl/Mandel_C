cmake_minimum_required(VERSION 3.7)

project(Mandel_C)

find_package(SDL2 REQUIRED)
find_package (Threads)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include/)

add_executable(Mandel_C main.cpp)

target_link_libraries(Mandel_C ${SDL2_LIBRARIES})
target_link_libraries (Mandel_C ${CMAKE_THREAD_LIBS_INIT})