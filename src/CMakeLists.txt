# Author: Jay Berg
# Date: March 17 2014

include_directories (.. ../include )
link_directories(lib)



file ( GLOB SRCS *.cpp *.h)

FIND_PACKAGE( OpenSSL )
find_package( BerkeleyDB )
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

add_executable( fantasybit ${SRCS} )
target_link_libraries(fantasybit libfc.a ${Boost_LIBRARIES} ${ALL_OPENSSL_LIBRARIES}) 