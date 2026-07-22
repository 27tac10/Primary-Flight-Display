cmake_minimum_required(VERSION 3.10)
project(PrimaryFlightDisplay VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(PrimaryFlightDisplay src/app/main.cpp src/core/SimulatedDataSource.cpp src/render/SDLBackend.cpp)

target_link_libraries(PrimaryFlightDisplay PRIVATE SDL2::SDL2 SDL2::SDL2main)

add_executable(PrimaryFlightDisplayTests tests/test_simulated_data_source.cpp src/core/SimulatedDataSource.cpp)
target_link_libraries(PrimaryFlightDisplayTests PRIVATE Catch2::Catch2WithMain)