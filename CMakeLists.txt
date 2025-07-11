cmake_minimum_required(VERSION 3.27)
project(pingpong)

set(CMAKE_CXX_STANDARD 23)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(pingpong main.cpp)

target_link_libraries(pingpong sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
