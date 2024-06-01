cmake_minimum_required(VERSION 3.1)

project(bgfx-lines)

set(CMAKE_CXX_STANDARD 17)
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(EXE_NAME main)

include(${PROJECT_SOURCE_DIR}/cmake/bgfx_utils.cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/bgfx.cmake)

include_directories(${INCLUDE_DIR})
add_executable(${EXE_NAME} ${SRC_DIR}/cube.cpp)

_add_bgfx_shader(
    ${PROJECT_SOURCE_DIR}/src/vc_cubes.sc,
    ${PROJECT_SOURCE_DIR}/include
    ${EXE_NAME}
)


target_link_libraries(${EXE_NAME} bgfx example-common fs_cubes.sc vs_cubes.sc)