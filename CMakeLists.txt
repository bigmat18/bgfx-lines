cmake_minimum_required(VERSION 3.7)

if(${CMAKE_VERSION} VERSION_LESS 3.29)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.29)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(bgfx-lines)

set(VCLIB_BUILD_MODULE_RENDER ON)
set(VCLIB_BUILD_EXAMPLES OFF)
set(VCLIB_BUILD_TESTS OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/vclib)

file(GLOB SOURCES "src/V2/*")
add_library(bgfx-lines ${SOURCES})
target_include_directories(bgfx-lines PUBLIC include/V2)
target_link_libraries(bgfx-lines PUBLIC vclib)

file(GLOB CPP_SOURCES "${CMAKE_SOURCE_DIR}/examples/V3/*.cpp")
message(STATUS "${CPP_SOURCES}")
add_executable(main ${CPP_SOURCES})
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/examples/V2)
target_link_libraries(main PUBLIC vclib bgfx-lines)


set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/shaders")
file(GLOB_RECURSE ABSOLUTE_SHADERS_FILES "${SHADERS_DIR}/*.sc")
set(RELATIVE_SHADERS_FILES "")

foreach(file ${ABSOLUTE_SHADERS_FILES})
    file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} ${file})
    list(APPEND RELATIVE_SHADERS_FILES ${relative_file})
endforeach()

message(STATUS "${RELATIVE_SHADERS_FILES}")
target_add_bgfx_shaders(
  main
  ${RELATIVE_SHADERS_FILES}
)
