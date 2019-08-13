cmake_minimum_required(VERSION 2.8)


project(utils C CXX)

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
#set(CXX_FLAGS
#  -g
#  -std=c++11
#  -D_FILE_OFFSET_BITS=64
#  -Wall
#  -Wold-style-cast
#  )
#
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g")
set(CMAKE_CXX_COMPILER g++)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(utils_src
  FileUtils.cpp
  Config.cpp)

add_library(utils ${utils_src})

add_subdirectory(test)
