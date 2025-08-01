cmake_minimum_required(VERSION 3.16)
project(markdown_viewer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    main.cpp
)

add_executable(markdown_viewer ${SOURCES})
