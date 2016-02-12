cmake_minimum_required(VERSION 2.8.4)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DNDEBUG=1 -Wall -Wextra -pedantic")

set(SOURCE_FILES client.cpp
				 ../common/client_socket.cpp


				 )
add_executable(client ${SOURCE_FILES})

include_directories(
	../
	./
	)