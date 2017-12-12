/*
 * (C) XuYuanChang 2017
 * 处理字符串
 */
#include "main.h"

char *str_upper(const char *str)
{
    char *origin;
    for (; *str != '\n'; *str++) {
        *origin = (char)toupper((int)*origin);
    }
    return origin;
}