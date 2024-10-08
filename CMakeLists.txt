cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.29)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.29)
endif()

project(bgfx-lines)
include(${PROJECT_SOURCE_DIR}/cmake/bgfx_utils.cmake)

message(STATUS "bgfx.cmake external library")
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/bgfx.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "src/*.cpp")
add_library(bgfx-lines ${SOURCES})
target_include_directories(bgfx-lines PUBLIC include)
target_link_libraries(bgfx-lines bx bgfx example-common)

add_executable(main main.cpp)

target_include_directories(main PUBLIC include)
target_link_libraries(main bgfx example-common bgfx-lines)


set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")
file(GLOB ABSOLUTE_SHADERS_FILES "${SHADERS_DIR}/*.sc")
set(RELATIVE_SHADERS_FILES "")

foreach(file ${ABSOLUTE_SHADERS_FILES})
    file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} ${file})
    list(APPEND RELATIVE_SHADERS_FILES ${relative_file})
endforeach()


target_add_bgfx_shaders(
  main
  ${RELATIVE_SHADERS_FILES}
)
