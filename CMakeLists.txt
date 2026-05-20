cmake_minimum_required(VERSION 3.10)

project(Bomberman)

set(CMAKE_CXX_STANDARD 17)

include_directories(C:/SFML/include)
link_directories(C:/SFML/lib)

add_executable(game main.cpp)

target_link_libraries(game sfml-graphics sfml-window sfml-system)