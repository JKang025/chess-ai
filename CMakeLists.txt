cmake_minimum_required(VERSION 3.17)
project(Virgo)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)

add_executable(Main main.cpp chess.cpp MCSTNode.cpp)

