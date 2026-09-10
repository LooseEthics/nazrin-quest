cmake_minimum_required(VERSION 3.25)

project(nazrin-quest
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
  src/main.cpp
  src/Maze.cpp
  src/MazeGenerator.cpp
)

add_executable(nazrin-quest ${SOURCES})

target_include_directories(nazrin-quest
  PRIVATE
  include
)

target_link_libraries(nazrin-quest
  PRIVATE
)
