cmake_minimum_required(VERSION 3.16)
project(Server)

set(CMAKE_CXX_STANDARD 17)


SET(THREADS "-pthread")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${THREADS}")

add_executable(Server server.cpp client.cpp socket.h main.cpp)