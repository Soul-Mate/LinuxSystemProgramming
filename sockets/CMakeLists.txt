include_directories(../includes)
# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH bin)
# 设置共享库输出路径
SET(LIBRARY_OUTPUT_PATH  ../libs)
# 添加library
AUX_SOURCE_DIRECTORY(../libs LIBS)
add_library(socketslib ${LIBS})
ADD_EXECUTABLE(in4_dgram_server src/in4_dgram_server.c)
ADD_EXECUTABLE(in4_dgram_client src/in4_dgram_client.c)
ADD_EXECUTABLE(in4_stream_server src/in4_stream_server.c)
ADD_EXECUTABLE(in4_stream_client src/in4_stream_client.c)
ADD_EXECUTABLE(in4_stream_server2 src/in4_stream_server2.c)
TARGET_LINK_LIBRARIES(in4_dgram_server socketslib)
TARGET_LINK_LIBRARIES(in4_dgram_client socketslib)
TARGET_LINK_LIBRARIES(in4_stream_server socketslib)
TARGET_LINK_LIBRARIES(in4_stream_client socketslib)
TARGET_LINK_LIBRARIES(in4_stream_server2 socketslib)
