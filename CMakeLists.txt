cmake_minimum_required(VERSION 3.1)

project(hello_world VERSION 1.0
                    PROJECT_DESCRIPTION "BGFX Lines"
                    LANGUAGE CXX)

include_directories(${PROJECT_SOURCE_DIR})
add_library(applib D foo.cpp)
add_executable(app main.cpp)
target_link_libraries(app applib)