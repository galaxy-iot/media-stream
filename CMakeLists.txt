cmake_minimum_required(VERSION 3.5)

# Set the project name
project (media-stream)

include_directories(
    include/net
)

add_library (net
    src/net/event.cpp
    src/net/nio_event_loop.cpp
    src/net/socket.cpp
    src/net/epoll_event_context.cpp
    src/net/tcp_server.cpp
)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Add an executable
add_executable(media-stream main.cpp)

target_link_libraries(media-stream net)