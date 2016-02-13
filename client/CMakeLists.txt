cmake_minimum_required(VERSION 2.8.4)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DNDEBUG=1 -Wall -Wextra -pedantic -fsanitize=undefined")

set(SOURCE_FILES client.cpp
				 ../common/client_socket.cpp
				 ../common/client_socket_manager.cpp
				 ../common/api.cpp
				 )
add_executable(client ${SOURCE_FILES})

include_directories(
	../
	./
	)

find_package(Threads)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})