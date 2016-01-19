cmake_minimum_required(VERSION 3.3)
project(TD5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    read_filtration.cpp)

add_executable(TD5 ${SOURCE_FILES})