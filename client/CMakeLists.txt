cmake_minimum_required(VERSION 2.8.4)
project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG=1 -Wall -Wextra -pedantic -fsanitize=undefined")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

set(SOURCE_FILES 
	../common/client_socket.cpp
	../common/client_socket_manager.cpp
	../common/communication_manager.cpp
	../common/api.cpp
	../common/client_api.cpp
	../common/server_api.cpp
	../common/file_scanner.cpp
	../common/file_diff.cpp
	../common/deleted_file_list.cpp

	client_logic.cpp
)
add_executable(client ${SOURCE_FILES} client.cpp)
add_executable(tests ${SOURCE_FILES} tests.cpp)

include_directories(
	../
	./
	${Boost_INCLUDE_DIRS}
)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53.0 COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(client 
							${CMAKE_THREAD_LIBS_INIT}
							${Boost_FILESYSTEM_LIBRARY}
							${Boost_SYSTEM_LIBRARY}
)
target_link_libraries(tests 
							${CMAKE_THREAD_LIBS_INIT}
							${Boost_FILESYSTEM_LIBRARY} 
							${Boost_SYSTEM_LIBRARY}
)

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
