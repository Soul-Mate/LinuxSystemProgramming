include_directories(../includes)
# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH bin)
# 设置共享库输出路径
SET(LIBRARY_OUTPUT_PATH  ../libs)
# 添加library
AUX_SOURCE_DIRECTORY(../libs LIBS)
add_library(libs ${LIBS})
ADD_EXECUTABLE(get_time_t src/get_time_t.c)
ADD_EXECUTABLE(get_tm src/get_tm.c)
ADD_EXECUTABLE(format_time src/format_time.c)
TARGET_LINK_LIBRARIES(get_time_t libs)
TARGET_LINK_LIBRARIES(get_tm libs)
TARGET_LINK_LIBRARIES(format_time libs)
