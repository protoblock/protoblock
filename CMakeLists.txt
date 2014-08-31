# Author: Jay Berg
# Date: March 17 2014
cmake_minimum_required(VERSION 2.8)

project(fantasybit CXX)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_CXX_FLAGS "-v -std=c++11 -stdlib=libc++ -g -Wall -O0 -pthread	")
#SET(CMAKE_CXX_FLAGS "-std=c++11")

IF(WIN32) 
add_definitions("-DFBWIN")
ENDIF()

set(PROTOBUF_LIBRARY libprotobuf)
set(FC_LIBRARY fc_debug)

add_subdirectory( src ) 
add_subdirectory( cli ) 
  