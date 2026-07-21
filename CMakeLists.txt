cmake_minimum_required(VERSION 3.10)
project(PrimaryFlightDisplay VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(PrimaryFlightDisplay main.cpp)

target_link_libraries(PrimaryFlightDisplay PRIVATE SDL2::SDL2 SDL2::SDL2main)