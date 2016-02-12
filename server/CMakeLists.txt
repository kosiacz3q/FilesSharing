cmake_minimum_required(VERSION 2.8.4)
project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DNDEBUG=1 -Wall -Wextra -pedantic")

set(SOURCE_FILES server.cpp)
add_executable(server ${SOURCE_FILES})

include_directories(
	../
	./
	)