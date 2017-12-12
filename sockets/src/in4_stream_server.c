/*
 * (C) XuYuanChang 2017
 * 一个简单的转换大小写服务器 基于TCP 支持IPV4
 */
#include "main.h"
#include "sock.h"
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{
    const  int SERVER_PORT = 8090;
    int    sfd,cfd,addrLen;
    struct sockaddr_in servAddr,clAddr;
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if (sfd < 0)
        err_msg("socket error");
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);
    if (bind(sfd,(struct sockaddr *)&servAddr,sizeof(struct sockaddr_in)) < 0 )
        err_msg("bind error");
    if (listen(sfd,128) < 0)
        err_msg("listen error");
    addrLen = sizeof(struct sockaddr_in);
    for(;;) {
        if ((cfd = accept(sfd,(struct sockaddr *)&clAddr,&addrLen)) < 0)
            err_msg("accept error");
        print_ip_and_port((const struct sockaddr *)&clAddr,"connect success");
    }
    return 0;
}