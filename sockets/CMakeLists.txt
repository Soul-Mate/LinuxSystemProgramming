include_directories(../includes)
# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH bin)
add_library(socketslib ../libs/readline.c)
ADD_EXECUTABLE(in4_dgram_server src/in4_dgram_server.c)
ADD_EXECUTABLE(in4_dgram_client src/in4_dgram_client.c)
TARGET_LINK_LIBRARIES(in4_dgram_server socketslib)
