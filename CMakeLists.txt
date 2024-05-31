cmake_minimum_required(VERSION 3.1)

project(bgfx-lines)

set (CMAKE_CXX_STANDARD 17)

set(SRC_DIR src)
set(INCLUDE_DIR include)

include_directories(${INCLUDE_DIR})

add_subdirectory(external/bgfx.cmake)
add_executable(app ${SRC_DIR}/main.cpp)

target_link_libraries(app bgfx example-common)