cmake_minimum_required(VERSION 3.25)

project(nazrin-quest
  VERSION 0.1
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.x
)

FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
  SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

glad_add_library(
  glad_gl_core_33
  STATIC
  REPRODUCIBLE
  API gl:core=3.3
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)

set(SOURCES
  src/main.cpp
  src/Maze.cpp
  src/MazeGenerator.cpp
  src/LogicMaze.cpp
  src/MazeMeshBuilder.cpp
  src/Renderer.cpp
  src/Camera.cpp
  src/Player.cpp
  src/Input.cpp
  src/CommonGeometry.cpp
  src/Game.cpp
)

add_executable(nazrin-quest ${SOURCES})

target_include_directories(nazrin-quest
  PRIVATE
  include
  ${stb_SOURCE_DIR}
)

target_link_libraries(nazrin-quest
  PRIVATE
  SDL3::SDL3
  glad_gl_core_33
  glm::glm
)

if(WIN32)
  add_custom_command(
    TARGET nazrin-quest
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:nazrin-quest>
  )
endif()

add_custom_command(
  TARGET nazrin-quest
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:nazrin-quest>/shaders
)
