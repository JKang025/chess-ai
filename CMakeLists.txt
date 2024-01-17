cmake_minimum_required(VERSION 3.17)
project(Virgo)

set(CMAKE_CXX_STANDARD 20)

add_executable(Main main.cpp chess.cpp MCSTNode.cpp)

# test to check max stack depth limit
# target_link_libraries(Virgo PRIVATE "-Wl,--stack,10000000")