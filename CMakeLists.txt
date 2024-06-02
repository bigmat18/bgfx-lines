cmake_minimum_required(VERSION 3.2)

project(bgfx-lines)
include(${PROJECT_SOURCE_DIR}/cmake/bgfx_utils.cmake)

message(STATUS "bgfx.cmake external library")
add_subdirectory(${PROJECT_SOURCE_DIR}/external/bgfx.cmake)

set(CMAKE_CXX_STANDARD 17)
add_executable(main src/cube.cpp)
target_link_libraries(main bgfx example-common)

set(SHADERS
    shaders/vs_base.sc
    shaders/fs_base.sc
    shaders/varying.def.sc)

target_add_bgfx_shaders(
  main
  ${SHADERS}
)