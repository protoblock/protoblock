# Author: Jay Berg
# Date: March 17 2014

include_directories (.. ../include ../include/nanomsg ../src ../include/tempheader)
#include_directories(${PROTOBUF_INCLUDE_DIR})
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

link_directories(../../lib)

#file(GLOB SRCPB ${CMAKE_CURRENT_BINARY_DIR}/../src/*.pb.*)
file (GLOB SRCS2 *.cpp *.h *.hpp *.cc)
#set_source_files_properties(${SRCPB} PROPERTIES GENERATED TRUE)

set(BOOST_DEBUG 1)
set(Boost_INCLUDE_DIR "C:/libs/boost_1_55_0")	
set(Boost_LIBRARY_DIR  "C:/libs/boost_1_55_0/stage/lib")

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


INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )

MESSAGE(WARNING " ssl ${OPENSSL_LIBRARIES}")


add_executable( fantasybit ${SRCS2} ${SRCPB} )
target_link_libraries(fantasybit fb ${FC_LIBRARY}  ${OPENSSL_LIBRARIES} nanomsg ${PROTOBUF_LIBRARY}  leveldb )
