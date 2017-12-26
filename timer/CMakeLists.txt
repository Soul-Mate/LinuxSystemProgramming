include_directories(../includes)
# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH bin)
# 设置共享库输出路径
SET(LIBRARY_OUTPUT_PATH  ../libs)
# 添加library
AUX_SOURCE_DIRECTORY(../libs LIBS)
add_library(timmerlibs ${LIBS})
ADD_EXECUTABLE(setitimer src/setitimer.c)
ADD_EXECUTABLE(getitimer src/getitimer.c)
ADD_EXECUTABLE(alarm src/alarm.c)
ADD_EXECUTABLE(user_alarm src/user_alarm.c)
ADD_EXECUTABLE(alarm_read src/alarm_read.c)
ADD_EXECUTABLE(clock_gettime src/clock_gettime.c)


TARGET_LINK_LIBRARIES(setitimer timmerlibs)
TARGET_LINK_LIBRARIES(getitimer timmerlibs)
TARGET_LINK_LIBRARIES(alarm timmerlibs)
TARGET_LINK_LIBRARIES(user_alarm timmerlibs)
TARGET_LINK_LIBRARIES(alarm_read timmerlibs)
TARGET_LINK_LIBRARIES(clock_gettime timmerlibs)
