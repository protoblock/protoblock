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

FIND_PACKAGE( OpenSSL ) 
set (Boost_USE_STATIC_LIBS TRUE)
find_package( Boost REQUIRED COMPONENTS
                   thread 
                   date_time
                   system 
                   filesystem 
                   program_options
                   serialization 
                   chrono
                   unit_test_framework  
                   context 
                   locale
                   coroutine)

MESSAGE(WARNING " boost ${Boost_INCLUDE_DIR} ${Boost_LIBRARIES} ${Boost_LIBRARY_DIRS} xxx")
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories( ${OPENSSL_INCLUDE_DIR} )
LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )
SET( ALL_OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE})

add_subdirectory( src ) 
add_subdirectory( cli ) 
add_subdirectory( fbd ) 
  