#string( TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE )

SET ( SOURCES ../src/priority_queue.cpp ../src/main.cpp)

SET( INCLUDES ../src/priority_queue.hpp )
 
add_executable(priority_queue ${SOURCES} ${INCLUDES})

