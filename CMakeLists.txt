# Author: Jay Berg
# Date: March 17 2014
cmake_minimum_required(VERSION 2.8)

project(fantasybit)

#set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvmgcc51")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -g3 -Wall -O0")

add_subdirectory( src ) 
add_subdirectory( cli ) 
#add_subdirectory( secp256k1 )   