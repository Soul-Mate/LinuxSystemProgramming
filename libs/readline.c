//
// Created by xyc on 2017/12/12.
//
#include "main.h"

int read_line(int fd, void *buffer, size_t n) {
    if (buffer == NULL || n <= 0)
        return -1;
    char ch;
    char *buf;
    int  readNum;
    int  totalRead = 0;
    buf = (char *)buffer;

    for(;;) {
        readNum = read(fd,&ch,1);
        if (readNum == -1)
            if (errno == EINTR)
                continue;
            else
                return -1;
        else if (readNum == 0)
            if (totalRead == 0)
                return 0;
            else
                break;
        else
            if (totalRead < n - 1) {
                totalRead++;
                *buf++ = ch;
            }
            if (ch == '\n')
                break;

    }
    *buf = '\0';
    return totalRead;
}
