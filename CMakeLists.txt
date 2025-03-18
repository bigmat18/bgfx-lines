cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(bgfx-lines)
include(${PROJECT_SOURCE_DIR}/cmake/bgfx_utils.cmake)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/bgfx.cmake)


set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")

file(GLOB_RECURSE SOURCES_FILES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE ABSOLUTE_SHADERS_FILES "${SHADERS_DIR}/*.sc" "${SHADERS_DIR}/*.sh")

set(RELATIVE_SHADERS_FILES "")
foreach(file ${ABSOLUTE_SHADERS_FILES})
    file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} ${file})
    list(APPEND RELATIVE_SHADERS_FILES ${relative_file})
endforeach()

add_library(bgfx-lines ${SOURCES_FILES})
target_include_directories(bgfx-lines PUBLIC include)
target_link_libraries(bgfx-lines PUBLIC bgfx)
target_add_bgfx_shaders(
    bgfx-lines
    ${RELATIVE_SHADERS_FILES}
)

set(EXAMPLES_DIR "${CMAKE_SOURCE_DIR}/examples")
file(GLOB EXAMPLE_DIRS RELATIVE "${EXAMPLES_DIR}" "${EXAMPLES_DIR}/*")
foreach(EXAMPLE_DIR ${EXAMPLE_DIRS})
    set(EXAMPLE_MAIN "${EXAMPLES_DIR}/${EXAMPLE_DIR}/main.cpp")
    if (EXISTS "${EXAMPLE_MAIN}")
        add_executable(${EXAMPLE_DIR} "${EXAMPLE_MAIN}")
        target_include_directories(${EXAMPLE_DIR} PUBLIC ${EXAMPLES_DIR}/${EXAMPLE_DIR})
        target_link_libraries(${EXAMPLE_DIR} PUBLIC bgfx-lines example-common)
    endif()
endforeach()





