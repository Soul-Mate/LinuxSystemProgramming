cmake_minimum_required(VERSION 2.8)
project(LinuxSystemProgramming)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# 添加子目录
add_subdirectory(sockets)
add_subdirectory(time)
add_subdirectory(threads)
