# Author: Jay Berg
# Date: March 17 2014

include(FindProtobuf)
find_package(Protobuf REQUIRED)

include_directories (.. ../include include/nanomsg)
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB ProtoFiles *.proto)
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})


link_directories(lib)

file ( GLOB SRCS *.cpp *.h *.hpp)

FIND_PACKAGE( OpenSSL )
#find_package( BerkeleyDB )
find_package( Boost 1.54 REQUIRED COMPONENTS
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

                   
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories( ${OPENSSL_INCLUDE_DIR} )
SET( ALL_OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE})

add_executable( fantasybit ${SRCS} ${ProtoSources} ${ProtoHeaders})
target_link_libraries(fantasybit libfc.a ${Boost_LIBRARIES} ${ALL_OPENSSL_LIBRARIES} 
        nanomsg
        ${PROTOBUF_LIBRARY} ) 