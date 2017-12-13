/*
 * (C) XuYuanChang 2017
 * 处理字符串
 */
#include "main.h"
#include <ctype.h>
char *str_upper(char *str)
{
    char *origin = str;
    for (;*str != '\0';str++) {
        *str = (char)toupper((int)*str);
    }
    return origin;
}