include_directories(../includes)
# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH bin)
# 设置共享库输出路径
SET(LIBRARY_OUTPUT_PATH  ../libs)
# 添加library
AUX_SOURCE_DIRECTORY(../libs LIBS)
add_library(timmerlibs ${LIBS})
ADD_EXECUTABLE(setitimer src/setitimer.c)
TARGET_LINK_LIBRARIES(setitimer timmerlibs)
