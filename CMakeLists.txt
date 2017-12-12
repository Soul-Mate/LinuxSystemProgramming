cmake_minimum_required(VERSION 2.8)
project(LinuxProgramming)
set(CMAKE_BUILD_TYPE release)
# 设置执行文件输出路径
SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/libs)
# 添加子目录
add_subdirectory(sockets)
