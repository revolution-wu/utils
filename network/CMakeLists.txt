cmake_minimum_required(VERSION 2.8)
project(network C CXX)
set(net_src
  EventLoop.cpp
  buffer.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall -O0 -Wno-unused-variable -pthread") 
set(CMAKE_CXX_COMPILER "g++")                                                                                                                          
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(network_static STATIC ${net_src})
add_library(network_shared SHARED ${net_src})

set_target_properties(network_static PROPERTIES OUTPUT_NAME "network")
set_target_properties(network_shared PROPERTIES OUTPUT_NAME "netwrok")

add_subdirectory(test)

