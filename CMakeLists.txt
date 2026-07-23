cmake_minimum_required(VERSION 3.10)
project(PrimaryFlightDisplay VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx REQUIRED)
find_path(SDL2_GFX_INCLUDE_DIR NAMES SDL2/SDL2_gfxPrimitives.h REQUIRED)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf REQUIRED)
find_path(SDL2_TTF_INCLUDE_DIR NAMES SDL2/SDL_ttf.h REQUIRED)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(PrimaryFlightDisplay 
                src/app/main.cpp 
                src/core/SimulatedDataSource.cpp 
                src/render/SDLBackend.cpp 
                src/core/Layout.cpp 
                src/instruments/AttitudeIndicator.cpp)
target_link_libraries(PrimaryFlightDisplay PRIVATE SDL2::SDL2 SDL2::SDL2main ${SDL2_GFX_LIBRARY})
target_include_directories(PrimaryFlightDisplay PRIVATE ${SDL2_GFX_INCLUDE_DIR})
target_link_libraries(PrimaryFlightDisplay PRIVATE SDL2::SDL2 SDL2::SDL2main ${SDL2_GFX_LIBRARY} ${SDL2_TTF_LIBRARY})
target_include_directories(PrimaryFlightDisplay PRIVATE ${SDL2_GFX_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_executable(PrimaryFlightDisplayTests tests/test_simulated_data_source.cpp src/core/SimulatedDataSource.cpp)
target_link_libraries(PrimaryFlightDisplayTests PRIVATE Catch2::Catch2WithMain)
