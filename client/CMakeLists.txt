cmake_minimum_required(VERSION 2.8.4)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG=1 -Wall -Wextra -pedantic -fsanitize=undefined")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

set(SOURCE_FILES 
				 ../common/client_socket.cpp
				 ../common/client_socket_manager.cpp
				 ../common/api.cpp
				 )
add_executable(client ${SOURCE_FILES} client.cpp)
add_executable(tests ${SOURCE_FILES} tests.cpp)

include_directories(
	../
	./
	)

find_package(Threads)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})