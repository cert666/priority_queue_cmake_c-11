cmake_minimum_required(VERSION 3.0)

PROJECT(priority_queue CXX)


if( MSVC )
	PROJECT(priority_queue CXX)

	SET(CMAKE_INSTALL_PREFIX, "./build_MSVC/" CACHE PATH "" FORCE)

	add_subdirectory("./build_MSVC/" )

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++11")

	install (TARGETS priority_queue RUNTIME DESTINATION ${PROJECT_BINARY_BIN}/MSVC)
else()
	SET(CMAKE_INSTALL_PREFIX, "./build_MinGW/" CACHE PATH "" FORCE)

	add_subdirectory("./build_MinGW/" )

	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++11")

endif()