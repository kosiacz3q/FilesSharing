cmake_minimum_required(VERSION 2.8.4)
project(server)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DNDEBUG=1 -Wall -Wextra -pedantic -fsanitize=undefined")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")



set(SOURCE_FILES 
	server.cpp 
	ServerSocket.cpp 
	ServerSocket.h
	ServerManager.cpp 
	ServerManager.h
	RuntimeContext.h
	RuntimeContext.cpp 
	RuntimeContext.h
	RuntimeContext.cpp 
	RuntimeContext.h 
	ClientsManager.cpp 
	ClientsManager.h 

	../common/client_socket.cpp
        ../common/client_socket_manager.cpp
	../common/communication_manager.cpp
	../common/api.cpp
)

add_executable(server ${SOURCE_FILES} server.cpp)

include_directories(
	../
	./
	)

find_package(Threads)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
