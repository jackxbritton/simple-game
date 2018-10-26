cmake_minimum_required(VERSION 3.5.1)

include_directories(src)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-g -Wall")

set(BIN sdl)

project(${BIN})
add_executable(${BIN} src/main.cpp src/game.cpp src/text.cpp src/vector_math.cpp)
target_link_libraries(${BIN} m SDL2 SDL2_ttf)
