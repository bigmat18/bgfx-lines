cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.29)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.29)
endif()

project(bgfx-lines)
include(${PROJECT_SOURCE_DIR}/cmake/bgfx_utils.cmake)

message(STATUS "bgfx.cmake external library")
add_subdirectory(${PROJECT_SOURCE_DIR}/external/bgfx.cmake)

set(CMAKE_CXX_STANDARD 17)
add_executable(main src/main.cpp)

target_include_directories(main PUBLIC include)
target_link_libraries(main bgfx example-common)

set(SHADERS
    shaders/vs_base.sc
    shaders/fs_base.sc
    shaders/varying.def.sc)

target_add_bgfx_shaders(
  main
  ${SHADERS}
)