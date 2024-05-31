cmake_minimum_required(VERSION 3.1)

project(bgfx-lines)

set (CMAKE_CXX_STANDARD 17)

set(SRC_DIR src)
set(INCLUDE_DIR include)
set(EXE_NAME main)

include_directories(${INCLUDE_DIR})

add_subdirectory(external/bgfx.cmake)

add_executable(${EXE_NAME} ${SRC_DIR}/cube.cpp)

target_link_libraries(${EXE_NAME} bgfx example-common)