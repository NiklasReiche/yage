cmake_minimum_required(VERSION 3.6)

project(YAGE CXX)

include_directories(external/include)
include_directories(source/core)

set(libPath ${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(source)