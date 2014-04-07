# Author: Jay Berg
# Date: March 17 2014

include_directories (../src/ ../include include/nanomsg)
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)

link_directories(lib)

file(GLOB SRCPB ${CMAKE_CURRENT_BINARY_DIR}/../src/*.pb.*)
file (GLOB SRCS2 *.cpp *.h *.hpp *.cc)
set_source_files_properties(${SRCPB} PROPERTIES GENERATED TRUE)


add_executable( fantasybitcli ${SRCS2} ${SRCPB} )
target_link_libraries(fantasybitcli fb nanomsg ${PROTOBUF_LIBRARY} ) 