cmake_minimum_required(VERSION 2.8)
project(LinuxSystemProgramming)
set(CMAKE_BUILD_TYPE release)
# 添加子目录
add_subdirectory(sockets)
add_subdirectory(time)
add_subdirectory(threads)
