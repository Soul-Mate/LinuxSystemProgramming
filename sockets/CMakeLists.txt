include_directories(../includes)
# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH bin)
# 添加library
set(LIBDIR ../libs)
add_library(socketslib ${LIBDIR}/readline.c ${LIBDIR}/sock.c ${LIBDIR}/error.c)
ADD_EXECUTABLE(in4_dgram_server src/in4_dgram_server.c)
ADD_EXECUTABLE(in4_dgram_client src/in4_dgram_client.c)
ADD_EXECUTABLE(in4_stream_server src/in4_stream_server.c)
ADD_EXECUTABLE(in4_stream_client src/in4_stream_client.c)
TARGET_LINK_LIBRARIES(in4_dgram_server socketslib)
TARGET_LINK_LIBRARIES(in4_dgram_client socketslib)
TARGET_LINK_LIBRARIES(in4_stream_server socketslib)
TARGET_LINK_LIBRARIES(in4_stream_client socketslib)
