# Author: Jay Berg
# Date: March 17 2014

include_directories (.. ../include ../include/nanomsg ../include/fb)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

link_directories(../../lib )

file ( GLOB SRCS *.cpp *.h *.hpp *.cc *.c)
set(BOOST_DEBUG 1)
set(Boost_INCLUDE_DIR "C:/libs/boost_1_55_0")	
set(Boost_LIBRARY_DIR  "C:/libs/boost_1_55_0/stage/lib")

FIND_PACKAGE( OpenSSL ) 
#find_package( BerkeleyDB )
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
#add_executable( fantasybit ${SRCS} )
add_library(fb ${SRCS} )

MESSAGE(WARNING " ssl ${OPENSSL_LIBRARIES}")

#SET(CPPNETLIBS cppnetlib-client-connections cppnetlib-uri cppnetlib-server-parsers)

#target_link_libraries(fantasybit ${FC_LIBRARY}  ${OPENSSL_LIBRARIES} nanomsg ${PROTOBUF_LIBRARY}  leveldb )
#target_link_libraries(fantasybit fb ${FC_LIBRARY} nanomsg ${PROTOBUF_LIBRARY} ) 
        
	